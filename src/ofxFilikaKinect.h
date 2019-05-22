#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"

class ofxFilikaKinect {
private:
	ofMesh mesh;
	ofxKinect * kinect;
	ofxOscSender sender;
	ofCamera cam;

	bool isUseSerial;

	// 2D Image conversion
	ofFbo meshToPix;
	ofFbo meshToPixCropped;

	// OpenCV image processing
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvContourFinder contourFinder;  // Blob Detection

	// Data stream buffer
	ofBuffer buffer;

	// tilt angle
	int angle;

public:
	ofxPanel gui;
	ofParameterGroup guiSensorFolder;
	ofParameterGroup guiContourFinder;
	ofParameterGroup guiMotorControlFolder;
	ofParameterGroup guiMeshFolder;
	ofParameterGroup guiBufferFolder;
	ofParameter<int> guiBufferThresNear;
	ofParameter<int> guiBufferThresFar;

	ofParameterGroup guiOSC;

	ofParameter<string> guiId;
	ofParameter<bool> guiBufferEnable;
	ofParameter<bool> guiVidDepthEnable;
	ofParameter<bool> guiVidRGBEnable;
	ofParameter<bool> guiMeshEnable;

	ofParameter<glm::vec2> guiBufferPos;
	ofParameter<glm::vec2> guiMeshPos;
	ofParameter<int> guiMeshStepSize;
	ofParameter<int> guiMeshPointSize;
	ofParameter<ofColor> guiMeshColor;
	ofParameter<int> guiSensorNear;
	ofParameter<int> guiSensorFar;


	// Motor tilt controller
	ofParameter<void> guiMotorUp;
	ofParameter<void> guiMotorDown;
	ofParameter<string> guiMotorAngle;


	// ROI
	ofParameter<glm::vec4> guiROI;

	// BLOB Finder
	ofParameter<bool> guiFindContours;
	ofParameter<int> guiCVNearThres;
	ofParameter<int> guiCVFarThres;
	ofParameter<int> guiMaxBlobSize;
	ofParameter<int> guiMinBlobSize;
	ofParameter<int> guiMaxNumBlobs;

	// OSC 
	ofParameter<bool> guiEnableOSC;
	ofParameter<string> guiOSCPort;
	ofParameter<string> guiOSCAdd;


	ofxFilikaKinect() {

	}

	~ofxFilikaKinect() {
		while (kinect->isConnected()) {
			kinect->close();
		}

		delete kinect;
		cout << "Sensor Id: " << guiId << " cleared from RAM << SUCCESS" << endl;

		Sleep(500);
	}



	/* GETTERS & SETTERS */
	ofMesh getDepthMesh() {

		int w = 640;
		int h = 480;

		mesh.clear();

		mesh.setMode(OF_PRIMITIVE_POINTS);
		for (int y = 0; y < h; y += guiMeshStepSize) {
			for (int x = 0; x < w; x += guiMeshStepSize) {
				if (kinect->getDistanceAt(x, y) > kinect->getNearClipping() && kinect->getDistanceAt(x, y) < kinect->getFarClipping()) {
					mesh.addColor(ofColor(guiMeshColor->r, guiMeshColor->g, guiMeshColor->b, guiMeshColor->a));
					mesh.addVertex(kinect->getWorldCoordinateAt(x, y));
				}
			}
		}

		return mesh;
	}



	string getSensorSerial() {
		return kinect->getSerial();
	}

	string getGuiName() {
		return guiSensorFolder.getName();
	}

	void setMeshColor(ofColor _cl) {
		guiMeshColor.set(ofColor(_cl.r, _cl.g, _cl.b, _cl.a));
	}


	void setStreamEnable(string host = "localhost", int port = 3333) {
		//guiEnableOSC = true;
		guiOSCAdd.set(host);
		guiOSCPort.set(ofToString(3333));
		if (guiEnableOSC) {
			//sender.setup(guiOSCAdd,ofToInt(guiOSCPort));
		}
	}

	void onGUIMotorMove(const void * sender) {
		ofParameter<void> * button = (ofParameter<void>*)sender;
		string nn = button->getName();

		if (nn == "Move Up") {
			angle++;
			if (angle > 30)
				angle = 30;
		}
		else {
			angle--;
			if (angle < -30)
				angle = -30;
		}

		guiMotorAngle.set(ofToString(angle));
		kinect->setCameraTiltAngle(angle);
	}

	void onChangeROI(glm::vec4 & e) {
		allocateROI();
	}

	void onChangeOSCBtn(bool & e) {
		if (e) {
			if ((guiOSCAdd.get() != "") || (guiOSCPort.get() != "")) {
				sender.setup(guiOSCAdd.get(), ofToInt(guiOSCPort.get()));
			}
			else {
				ofSystemAlertDialog("IP Address or PORT not found");
				guiEnableOSC.set(false);
			}
		}
		else
		{
			sender.clear();
		}
	}

	void setup(string _id = "", bool _isUseSerial = false) {

		isUseSerial = _isUseSerial;
		guiId = _id;
		guiBufferEnable = false;
		// GUI
		gui.setDefaultWidth(250);
		gui.setup("SENSORS_GUI", "settings_ofxFilikaKinect.xml");
		guiSensorFolder.setName("Sensor-" + guiId.get());
		guiContourFinder.setName("Contour Finder");

		guiROI.set("ROI", glm::vec4(100, 300, 600, 200), glm::vec4(0, 0, 0, 0), glm::vec4(2000, 2000, 2000, 2000));
		guiSensorNear.set("Sensor Near Thres", 200, 0, 255);
		guiSensorFar.set("Sensor Far Thres", 70, 0, 255);

		guiMeshEnable.set("Draw Mesh", false);
		guiMeshPointSize.set("Mesh Point Size", 3, 1, 8);
		guiMeshStepSize.set("Mesh Vert. Step", 4, 1, 16);
		guiMeshPos.set("Mesh Pos", glm::vec2(0, 0), glm::vec2(-1000, -1000), glm::vec2(1000, 1000));
		guiBufferPos.set("Buffer Pos", glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(ofGetWidth(), ofGetHeight()));

		guiMeshColor.set("Mesh Color", ofColor(255, 255, 255, 100), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
		guiBufferEnable.set("Draw Buffer", false);
		guiBufferThresNear.set("Buffer Near Thres", 230, 0, 255);
		guiBufferThresFar.set("Buffer Far Thres", 70, 0, 255);

		guiVidDepthEnable.set("Draw Depth Video", true);
		guiVidRGBEnable.set("Draw RGB Video", true);

		guiFindContours.set("Find Contours", false);
		guiSensorNear.set("Sensor Near Thres", 500, 0, 3000);
		guiSensorFar.set("Sensor Far Thres", 4000, 0, 6000);

		guiMaxNumBlobs.set("Max Blob Number", 20, 1, 50);
		guiMinBlobSize.set("Min Blob Size", 100, 10, 2000);
		guiMaxBlobSize.set("Max Blob Size", 2000, 10, 2000);

		// Sensor Motor Tilt Control
		guiMotorDown.addListener(this, &ofxFilikaKinect::onGUIMotorMove);
		guiMotorUp.addListener(this, &ofxFilikaKinect::onGUIMotorMove);

		guiMotorUp.set("Move Up");
		guiMotorDown.set("Move Down");
		guiMotorAngle.set("Tilt Angle","0");
		// OSC
		guiOSC.setName("OSC Parameters");
		guiEnableOSC.set("Enable OSC", false);
		guiOSCAdd.set("IP", "192.168.1.11");
		guiOSCPort.set("PORT", "3333");

		// GUI MESH FOLDER
		guiMeshFolder.setName("MESH SETTINGS");
		guiMeshFolder.add(guiMeshEnable);
		guiMeshFolder.add(guiMeshPointSize);
		guiMeshFolder.add(guiMeshStepSize);
		guiMeshFolder.add(guiMeshPos);
		guiMeshFolder.add(guiMeshColor);


		// Add Controller to view
		
		guiSensorFolder.add(guiSensorNear);
		guiSensorFolder.add(guiSensorFar);
		guiSensorFolder.add(guiROI);

		// BUFFER FOLDER
		guiBufferFolder.setName("BUFFER IMAGE SETTINGS");
		guiBufferFolder.add(guiBufferEnable);
		guiBufferFolder.add(guiBufferThresNear);
		guiBufferFolder.add(guiBufferThresFar);
		guiBufferFolder.add(guiBufferPos);

		// Add Motor Control
		guiMotorControlFolder.setName("Sensor Motor Control");
		guiMotorControlFolder.add(guiMotorDown);
		guiMotorControlFolder.add(guiMotorUp);
		guiMotorControlFolder.add(guiMotorAngle);
		guiSensorFolder.add(guiMotorControlFolder);

		guiSensorFolder.add(guiVidDepthEnable);
		guiSensorFolder.add(guiVidRGBEnable);

		// contour Finder
		guiContourFinder.add(guiFindContours);
		guiContourFinder.add(guiMaxNumBlobs);
		guiContourFinder.add(guiMinBlobSize);
		guiContourFinder.add(guiMaxBlobSize);

		guiSensorFolder.add(guiContourFinder);

		// Add OSc
		guiSensorFolder.add(guiOSC);
		guiOSC.add(guiEnableOSC);
		guiOSC.add(guiOSCAdd);
		guiOSC.add(guiOSCPort);
		// Add listener for ROI value changes
		guiROI.addListener(this, &ofxFilikaKinect::onChangeROI);
		guiEnableOSC.addListener(this, &ofxFilikaKinect::onChangeOSCBtn);
		guiEnableOSC.disableEvents();

		gui.add(guiMeshFolder);
		gui.add(guiBufferFolder);
		gui.add(guiSensorFolder);
	
		gui.getGroup("Sensor-").setDefaultHeaderBackgroundColor(ofColor::greenYellow);
		gui.getGroup("Sensor-").setDefaultFillColor(ofColor::greenYellow);
		gui.getGroup("Sensor-").setDefaultBorderColor(ofColor::greenYellow);
		gui.getGroup("Sensor-").setDefaultBackgroundColor(ofColor(0, 0, 0, 200));

		
		gui.getGroup("Sensor-").minimizeAll();
		gui.getGroup("MESH SETTINGS").minimizeAll();
		gui.getGroup("MESH SETTINGS").minimize();
		

		// Load previous settings
		gui.loadFromFile("settings_ofxFilikaKinect.xml");

		// If OSC Enabled, then start osc
		if (guiEnableOSC) {
			bool enableO = true;
			onChangeOSCBtn(enableO);
		}
		

		// init Kinect
		kinect = new ofxKinect();

		while (kinect->isConnected() != true) {
			kinect->init(false, true);

			if (isUseSerial) {
				kinect->open(guiId);

			}
			else {
				kinect->open();
			}
		}

		//cout << "Sensor " << guiId << " is ready" << endl;

		allocateROI();

		// Set the last motor tilt angle
		//cout << guiMotorAngle << endl;
		angle = ofToInt(guiMotorAngle);
		kinect->setCameraTiltAngle(angle);
	}

	void allocateROI() {
		// Clear FBOs & CVImages
		meshToPix.clear();
		meshToPixCropped.clear();
		colorImg.clear();
		grayImage.clear();
		
		// Reallocate
		meshToPix.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
		meshToPixCropped.allocate(guiROI->z, guiROI->w, GL_RGB);
		colorImg.allocate(guiROI->z, guiROI->w);
		grayImage.allocate(guiROI->z, guiROI->w);

		buffer.clear();
		buffer.allocate(guiROI->z * guiROI->w);
	}

	void update() {
		if (kinect->isConnected()) {
			kinect->update();
			kinect->setDepthClipping(guiSensorNear, guiSensorFar);
		}
	}

	void draw(int _x = 0, int _y = 0) {
		ofSetColor(255, 0, 0, 100);
		ofDrawRectangle(_x, _y, 160, 120);
		ofDrawRectangle(_x, _y + 120, 160, 120);
		ofSetColor(255, 255, 255);

		if (kinect->isConnected()) {
			if (guiVidRGBEnable)
				kinect->draw(_x, _y, 160, 120);

			if (guiVidDepthEnable)
				kinect->drawDepth(_x, _y + 120, 160, 120);
		}

		
		if (guiBufferEnable || guiEnableOSC || guiFindContours)
		{

			meshToPix.begin();
			ofClear(0, 0);
			drawFilteredDepth(kinect->getDepthPixels()).draw(0, 0);
			meshToPix.end();
			
			// Crop to ROI
			meshToPixCropped.begin();
			ofClear(0, 0);
			meshToPix.getTexture().drawSubsection(0, 0, guiROI->z, guiROI->w, guiROI->x, guiROI->y, guiROI->z, guiROI->w);
			meshToPixCropped.end();


			// load grayscale depth image from the kinect source
			ofPixels meshToPixels;

			meshToPixCropped.readToPixels(meshToPixels);
			// Set color image pixels
			colorImg.setFromPixels(meshToPixels);
			// Set gray image from color image
			grayImage.setFromColorImage(colorImg);

			//dilate image to reduce noise
			colorImg.dilate();
			grayImage = colorImg;

			// update the cv images
			grayImage.flagImageChanged();
		}

		if (guiBufferEnable) {

			ofPushMatrix();
			//ofScale(-1,1);
			ofTranslate(guiBufferPos->x , guiBufferPos->y, 0);
			meshToPixCropped.draw(guiROI->x, guiROI->y);
			//region of interest
			ofPushStyle();
			ofSetColor(ofColor(guiMeshColor->r, guiMeshColor->g, guiMeshColor->b, 255));
			ofNoFill();
			ofDrawRectangle(guiROI->x, guiROI->y, guiROI->z, guiROI->w);
			ofPopStyle();
			
			ofPopMatrix();
		}


		if (guiFindContours) {

			contourFinder.findContours(grayImage, guiMinBlobSize, guiMaxBlobSize, guiMaxNumBlobs, false);
			//
			ofPushMatrix();
			//ofScale(-1,1);
			ofTranslate(guiBufferPos->x, guiBufferPos->y, 0);
			contourFinder.draw(guiROI->x, guiROI->y);
			ofPopMatrix();
		}


		if (guiEnableOSC && grayImage.width > 0) {
			buffer.clear();
			ofxCvGrayscaleImage img;
			img.allocate(int(guiROI->z), int(guiROI->w));
			img = grayImage;
			img.resize(int(guiROI->z * 0.5), int(guiROI->w * 0.5));
			//img.width = int( guiROI->z * 0.5 );
			//img.height = int( guiROI->w * 0.5 );

			
			ofSaveImage(img.getPixels(), buffer);
			//cout << "img w " << img.getWidth() << "img h " << img.getHeight() << "size " << buffer.size() << endl;
			ofxOscMessage m;
			m.setAddress("/kinectimage");
			m.addBlobArg(buffer);
			sender.sendMessage(m, false);

		}


		if (guiMeshEnable) {
			cam.begin();
			ofPushMatrix();
			ofTranslate(guiMeshPos->x, guiMeshPos->y, 0);
			drawPointCloud(getDepthMesh());
			ofPopMatrix();
			cam.end();
		}
		gui.draw();
		gui.setPosition(ofGetWidth() - gui.getWidth(), 0);
	}

	ofxCvGrayscaleImage drawFilteredDepth(ofPixels & px) {
		ofxCvGrayscaleImage imgThreshNear; // the near thresholded image
		ofxCvGrayscaleImage imgThreshFar; // the far thresholded image
		ofxCvGrayscaleImage imgThres;

		imgThres.allocate(kinect->getWidth(), kinect->getHeight());
		imgThreshFar.allocate(kinect->getWidth(), kinect->getHeight());
		imgThreshNear.allocate(kinect->getWidth(), kinect->getHeight());

		imgThres.setFromPixels(px);
		imgThreshFar = imgThres;
		imgThreshNear = imgThres;

		imgThreshNear.threshold(guiBufferThresNear, true);
		imgThreshFar.threshold(guiBufferThresFar);
		cvAnd(imgThreshNear.getCvImage(), imgThreshFar.getCvImage(), imgThres.getCvImage(), NULL);

		return imgThres;
	}

	void drawPointCloud(ofMesh mesh) {

		glPointSize(guiMeshPointSize);
		ofPushMatrix();
		// the projected points are 'upside down' and 'backwards' 
		ofScale(1, -1, -1);

		ofTranslate(0, 0, 2500); // center the points a bit
		ofEnableDepthTest();
		mesh.drawVertices();

		ofDisableDepthTest();
		ofPopMatrix();
	}

	void closeSensor() {
		while (kinect->isConnected()) {
			kinect->close();
		}

		cout << "Sensor Id: " << guiId << " terminated - SUCCESS" << endl;

		Sleep(500);
	}
};