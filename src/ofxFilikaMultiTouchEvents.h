#pragma once
#include "ofMain.h"

class ofxFilikaMultiTouchEvents {
	
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
