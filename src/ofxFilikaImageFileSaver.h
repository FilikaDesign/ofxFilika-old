#pragma once
#include "ofMain.h"
#include "ofMain.h"
#include "globalConstants.h"
#include "ofxIO.h"



class ofxFilikaImageFileSaver: public ofThread
{
public:

	string fileFormat;

	// Getters && Setters
	string getFileName() {

		return _fileName;
	};

	ofPixels pixels;

	void setup() {
		//default
		this->setImageFileFormat("jpg");
	};
	void saveImageFile(string folderName, ofPixels pixels) {


		_fileName = folderName + "/" + ofGetTimestampString() + "." + fileFormat;
		//set global name
		attachmentFile = _fileName;
		ofSaveImage(pixels, _fileName);

		try
		{
			std::string path = ofToDataPath(_fileName, true);
			ofx::IO::ByteBuffer byteBuffer;
			bool result = ofx::IO::ByteBufferUtils::loadFromFile(path, byteBuffer);
			if (result) {
				ofLogNotice("ofApp::setup") << "File written";
			}
			else {
				ofLogError("ofApp::setup") << "File not written";
			}
		}
		catch (Poco::Exception& exception)
		{
			ofLogError("ofApp::setup") << "Exception: " << exception.displayText();
		}
	};

	void setImageFileFormat(string _fileFormat) {
		fileFormat = _fileFormat;
	};

	void threadedFunction() {
		while (isThreadRunning())
		{
			// Attempt to lock the mutex.  If blocking is turned on,
			if (lock())
			{
				ofSaveImage(pixels, folderName + "/" + _fileName);
				//ofLog() << "is thread stopped";
				unlock();
				stopThread();
			}
		}

	};

	ofPixels getPixels() {

		unique_lock<std::mutex> lock(mutex);
		return pixels;

	};




private:
	string _fileName;
	string folderName;
};

