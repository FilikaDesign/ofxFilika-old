#pragma once
#include "ofxFilikaConstants.h"

class ofxFilikaEvents
{
public:
	ofEvent<void> ON_RELEASE;
	ofEvent<void> ON_PRESS;
	ofEvent<void> ON_HOVER;

	ofEvent<void> ON_COUNTER_DONE;
	//ofEvent<int> ON_COUNTER_TICK;

	ofEvent<string> ON_TEXT_RETURN;
	ofEvent<int> ON_INT_RETURN;

	// Inherits from "ofxFilikaConstants
	// SWIPE_UP
	// SWIPE_DOWN
	// SWIPE_LEFT
	// SWIPE_RIGHT
	ofEvent<ofxFilikaTouchGestures> ON_SWIPE;
};
