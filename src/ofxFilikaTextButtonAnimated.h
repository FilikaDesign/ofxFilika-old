#pragma once
//
#include "ofMain.h"
#include "ofxFilikaEvents.h"
#include "ofxTextSuite.h"
#include "ofxTweenzor.h"

class ofxFilikaTextButtonAnimated: public ofxFilikaEvents {

private:
	ofxTextBlock        myText;
	TextBlockAlignment  alignment;  //constants for controlling state
	ofFbo mask;
	ofFbo textFbo;
	float animateValX;
	float animateEndVal;

	float tAlpha;
	float tAnimPosX;

public:
	//ofEvent<int> BUTTON_CLICK;
	//ofEvent<int> BUTTON_TOUCH;
	//ofEvent<int> BUTTON_TOUCH_UP;

	int artistdbID; // Database Unique ID

	int wt, ht;
	ofColor mainColor;
	string txt;
	int size;
	float scaleFac;
	bool isMouseEnabled;
	bool isTouchEnabled;
	bool result;
	bool isSelected;
	bool isEnabledInteraction;

	//ofTrueTypeFont* f;
	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	void setTouchEnable(bool _val) {
		isTouchEnabled = _val;

		if (_val) {
			ofRegisterTouchEvents(this);
			if (isMouseEnabled) {
				isMouseEnabled = false;
				ofUnregisterMouseEvents(this);
			}
		}
		else {
			ofUnregisterTouchEvents(this);
			if (!isMouseEnabled) {
				isMouseEnabled = true;
				ofRegisterMouseEvents(this);
			}
		}
	}

	void setSelected(bool _val) {
		isSelected = _val;
		if (_val) {
			mainColor = ofColor(208, 142, 0);
		}
		else {
			mainColor = ofColor(80, 81, 81);
		}
	}

	void setAnimatable(bool _isAnimatable, float _del = 0) {

		isAnimatable = _isAnimatable;
		delay = _del;

		if (isAnimatable) {
			/*params2.setup(&x1, x1, x2, _del, 0.4f, EASE_OUT_SINE);
			Tweenzor::add(params2);

			Tweenzor::addCompleteListener(Tweenzor::getTween(params2.property), this, &ofxFilikaColorButton::onCompleteAnim);*/
		}

	}

	void animate() {
		mainColor = ofColor(80, 81, 81);;

		animateValX = -wt;
		animateEndVal = animateValX;

		tAnimPosX = -wt;
		tAlpha = 0;

		int nColumns = 1;
		int rowNum = floor(bId / nColumns);

		Tweenzor::add(&tAlpha, 0.0, 255.0, log(rowNum + 1) * 0.15, 2.2f, EASE_OUT_EXPO);
		Tweenzor::add(&tAnimPosX, tAnimPosX, 0.0, log(rowNum + 1) * 0.15, 0.5f, EASE_OUT_EXPO);
	}

	ofColor getColor() {
		return mainColor;
	}

	int getWidth() {
		return wt;
	}

	float getAnimationValX() {
		return animateValX;
	}
	string getTxt() {
		return txt;
	}

	void setContent(string _txt, int _artistdbID) {
		artistdbID = _artistdbID;

		myText.setText(_txt);
		myText.wrapTextX(wt);


		ht = myText.getHeight() + size;


		mask.clear();
		mask.allocate(wt, ht, GL_RGBA);
		mask.begin();
		ofDrawRectangle(0, 0, wt, ht); //added 10 to include the bottom missing character section
		mask.end();

		textFbo.clear();
		textFbo.allocate(wt, ht, GL_RGBA); //added 10 to include the bottom missing character section


		animate();
	}
	////////////////////////////////////////////////
	// CONSTRUCTOR
	////////////////////////////////////////////////
	ofxFilikaTextButtonAnimated() {
		bId = 0;
		isSelected = false;
	}

	// deconstructer
	~ofxFilikaTextButtonAnimated() {
		//delete f;
		//cout << "I'm deleted" << endl;
		//Sleep(2);

		Tweenzor::removeTween(&tAlpha);
		Tweenzor::removeTween(&tAnimPosX);
	}


	////////////////////////////////////////////////
	// SETUP
	////////////////////////////////////////////////
	void setup(ofTrueTypeFont * _f, string _txt, int _txtWidth, ofColor _mainColor, int _lineHAdjust, int _fontSize, int _id = 0, bool _isAnimatable = true) {

		tAlpha = 0;

		txt = _txt;

		//f = _f;
		wt = _txtWidth;

		myText.init(_f);
		myText.setLineHeight(_fontSize);
		myText.setText(txt);
		myText.wrapTextX(wt);

		ofSetCircleResolution(wt);

		//#ifdef TOUCH_ENABLE
		ofRegisterTouchEvents(this);
		isTouchEnabled = true;
		//#else
				// register events for interaction
		ofRegisterMouseEvents(this);
		isMouseEnabled = true;
		//#endif
		mainColor = _mainColor;
		size = _lineHAdjust;
		bId = _id;
		isAnimatable = _isAnimatable;
		scaleFac = 1.0;
		/*if(isAnimatable)
			Tweenzor::init();*/


		ht = myText.getHeight() + size;

		mask.allocate(wt, ht, GL_RGBA);
		mask.begin();
		ofDrawRectangle(0, 0, wt, ht); //added 10 to include the bottom missing character section
		mask.end();

		textFbo.allocate(wt, ht, GL_RGBA); //added 10 to include the bottom missing character section

		animateValX = -wt;
		animateEndVal = animateValX;

		tAnimPosX = -wt;



	}

	////////////////////////////////////////////////
	// HIT DETECTION
	////////////////////////////////////////////////
	bool hit(int _x, int _y) {

		result = (_x < xpos + wt && _x > xpos && _y > ypos - ht * 0.5 && _y < ypos + ht * 0.5) ? true : false;

		return result;
	}


	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	void draw(int _x, int _y) {

		xpos = _x;
		ypos = _y;

		ofPushStyle();
		//ofSetColor(ofColor::red);
		//ofDrawRectangle(_x, _y, wt, ht);
		ofPopStyle();
		
		ofPushMatrix();
		ofTranslate(_x, _y);
		ofScale(scaleFac, scaleFac);

		//ofScale(abs(sin(ofGetElapsedTimef())*2),abs(sin(ofGetElapsedTimef())*2));
		ofPushStyle();
		ofEnableAlphaBlending();

		ofSetColor(ofColor(gRed.r, gRed.g, gRed.b, tAlpha));
		int moveFac;
		if (myText.lines.size() > 1) {
			moveFac = -25;
		}
		else {
			moveFac = -15;
		}
		ofDrawRectRounded(ofRectangle( - 9, moveFac, 4, ht - size), 20, 20, 20, 20);

		/*if (animateValX < animateEndVal) {
			animateValX
		}*/
		ofPopStyle();
		//animateValX += (animateEndVal - animateValX) * 0.25;
		ofPushStyle();

		// Debugging
		/*ofSetColor(ofColor::red);
		ofDrawRectangle(-wt * 0.5, -ht * 0.5, wt, ht);*/

		ofEnableAlphaBlending();
		textFbo.begin();
		ofClear(0, 0);
		myText.setColor(mainColor.r, mainColor.g, mainColor.b, mainColor.a);
		myText.draw(tAnimPosX, 0);
		textFbo.end();

		textFbo.getTexture().setAlphaMask(mask.getTexture());

		textFbo.draw(0, -ht * 0.5);

		/*if (isSelected) {
			ofScale((1/ scaleFac), (1 / scaleFac));
			ofDrawRectRounded(-(wt + 6) * 0.5, ht*0.5 + 5, (wt + 6), 2, 4);
		}*/
		ofPopStyle();
		ofPopMatrix();

	}



	////////////////////////////////////////////////
	// INTERACTION
	////////////////////////////////////////////////
	//--------------------------------------------------------------
	void touchDown(ofTouchEventArgs & touch) {
		hitBegin(touch.x, touch.y);
	}

	//--------------------------------------------------------------
	void touchMoved(ofTouchEventArgs & touch) {
		hit(touch.x, touch.y);
		if (!result) {
			scaleFac = 1;
			isDown = false;
		}
	}

	//--------------------------------------------------------------
	void touchUp(ofTouchEventArgs & touch) {
		hitReleased(touch.x, touch.y);
	}

	//--------------------------------------------------------------
	void touchDoubleTap(ofTouchEventArgs & touch) {
	}

	//--------------------------------------------------------------
	void touchCancelled(ofTouchEventArgs & touch) {
		result = false;
	}


	//--------------------------------------------------------------
	void mouseScrolled(ofMouseEventArgs & args) {

	}

	//--------------------------------------------------------------
	void mouseEntered(ofMouseEventArgs & args) {

	}

	//--------------------------------------------------------------
	void mouseExited(ofMouseEventArgs & args) {

	}
	//--------------------------------------------------------------
	void mouseMoved(ofMouseEventArgs & args) {
		hit(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mouseDragged(ofMouseEventArgs & args) {
		hit(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mousePressed(ofMouseEventArgs & args) {
		hitBegin(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mouseReleased(ofMouseEventArgs & args) {

		hitReleased(args.x, args.y);
	}

	////////////////////////////////////////////////
	// CONTAINER RELEASED HANDLER
	////////////////////////////////////////////////
	void hitReleased(float touch_x, float touch_y) {
		hit(touch_x, touch_y);
		if (result && isDown) {
			ofNotifyEvent(ON_INT_RETURN, artistdbID,this);
			isDown = false;
		}

		mainColor = ofColor(80, 81, 81);
		if (!isSelected)
			scaleFac = 1.0;
	}

	////////////////////////////////////////////////
	// CONTAINER PRESSED HANDLER
	////////////////////////////////////////////////
	void hitBegin(float touch_x, float touch_y) {
		hit(touch_x, touch_y);
		if (result) {

			mainColor = ofColor(177, 41, 46);

			if (!isSelected)
				scaleFac = 1;

			//ofNotifyEvent(ON_PRESS, bId);

			isDown = true;
		}
	}

	////////////////////////////////////////////////
	// DISABLE INTERACTION
	////////////////////////////////////////////////
	void disableInteraction() {
		if (isEnabledInteraction) {
			if (isMouseEnabled)
				ofUnregisterMouseEvents(this);

			if (isTouchEnabled)
				ofUnregisterTouchEvents(this);
			isEnabledInteraction = false;
		}
	}

	////////////////////////////////////////////////
	// ENABLE INTERACTION
	////////////////////////////////////////////////
	void enableInteraction() {
		if (!isEnabledInteraction) {
			if (isMouseEnabled)
				ofRegisterMouseEvents(this);

			if (isTouchEnabled)
				ofRegisterTouchEvents(this);
			isEnabledInteraction = true;
		}

	}

private:
	int xpos, ypos, w, hitx, hity, bId, h;
	bool isAnimatable, delay, isDown;


	// Tween Lib
	float x1, x2;

	//TweenParams params2;
};