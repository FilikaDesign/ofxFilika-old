#pragma once
#include "ofMain.h"
#include "ofxFilikaConstants.h"

class ofxFilikaMultiTouchEvents {
public:
	ofEvent<ofxFilikaSwipeEvent> SWIPE_EVENT;
};

class ofxFilikaZoomEvent {
public:
	float distance;
	glm::vec2 center;
	float scale;
};


class ofxFilikaRotationEvent {
public:
	glm::vec2 center;
	float rotation;

};

class ofxFilikaSwipeEvent {
public:
	ofxFilikaTouchGestures swipeDir;
	int touchNum;
};
