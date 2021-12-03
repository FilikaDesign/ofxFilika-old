#pragma once

class ofxFilikaEvents
{
public:
	ofEvent<void> ON_RELEASE;
	ofEvent<void> ON_PRESS;
	ofEvent<void> ON_HOVER;

	ofEvent<void> ON_COUNTER_DONE;
	//ofEvent<int> ON_COUNTER_TICK;

	ofEvent<string> ON_TEXT_RETURN;

};

/*
#include "ofMain.h"

class imageViewInfo {
public:
	string vidSource;
	int btnId;
};

static ofEvent<imageViewInfo> IMAGE_ITEM_CLICKED;
*/