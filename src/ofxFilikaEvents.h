#pragma once
class sceneInfo {
public:
	int preId, currentId;
};
class ofxFilikaEvents
{
public:
	ofEvent<void> ON_RELEASE;
	ofEvent<void> ON_PRESS;
	ofEvent<void> ON_HOVER;

	ofEvent<void> ON_COUNTER_DONE;
	//ofEvent<int> ON_COUNTER_TICK;

	// SCENE EVENTS
	ofEvent<sceneInfo> ON_SWITCH_SCENE;

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