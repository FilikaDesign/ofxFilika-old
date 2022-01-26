#pragma once
#include "ofMain.h"
#include "ofxFilikaInteractiveContainer.h"
#include "ofxFilikaButtonIMG.h"
#include "ofxFilikaEvents.h"

class ofxFilikaSwipeButton : public ofxFilikaButtonIMG {
public:
	int btnId;

	 void setup(string src, int btnId, float _sFac) {
		ofxFilikaButtonIMG::setup(src);
		this->btnId = btnId;
		sFac = _sFac;
	}


	 void onPress(int x, int y, int button) {
		//ofxFilikaButtonIMG::onPress(x, y, button);
		printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		//Tweenzor::add(&sFac, sFac, 0.5, 0.0, 0.5f, EASE_OUT_EXPO);
		//ofNotifyEvent(ON_PRESS);
		//int id = ofToInt(paintingID);
		//ofNotifyEvent(ON_INT_RETURN, btnId, this);
	}

	virtual void onMouseMove(int x, int y) {
		//ofLog() << "x:" << x << "y:" << y;
	}

	virtual void onDragOver(int x, int y, int button) {
		//ofLog() << "Dragging x:" << x << "y:" << y;
	}

	 void onRelease(int x, int y, int button) {
		//ofxFilikaButtonIMG::onRelease(x, y, button);
		printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
		//Tweenzor::add(&sFac, sFac, 0, 0.0, 0.5f, EASE_OUT_EXPO);
		//sFac = 0;
		//ofNotifyEvent(ON_RELEASE);
	}

	 void onReleaseOutside(int x, int y, int button) {
		//ofxFilikaButtonIMG::onReleaseOutside(x, y, button);

		printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
		//sFac = 0;
		//Tweenzor::add(&sFac, sFac, 0, 0.0, 0.5f, EASE_OUT_EXPO);
		//ofNotifyEvent(ON_RELEASE);
	}
};