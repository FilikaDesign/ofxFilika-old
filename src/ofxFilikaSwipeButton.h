#pragma once
#include "ofMain.h"
#include "ofxFilikaButtonIMG.h"
#include "ofxFilikaEvents.h"

class ofxFilikaSwipeButton : public ofxFilikaButtonIMG {
private:
	float globalMagnitudeThreshold = 150;
	float verticalSpeedThreshold = -10;
public:
	int btnId;
	float orgSFac;
	bool isDownStart = false;
	glm::vec2 offSet;
	glm::vec2 touchStart, touchEnd, swipeSpd;

	virtual void setup(string src, int btnId, float _sFac) {
		ofxFilikaButtonIMG::setup(src);
		this->btnId = btnId;
		orgSFac = _sFac;
	}

	virtual void draw(int x, int y) {
		ofxFilikaButtonIMG::draw();

		if (isDownStart) {
			this->x = x - offSet.x;
			this->y = y - offSet.y;
		}
		else {
			this->x = x;
			this->y = y;
		}

		if (this->y - this->width*0.5 < 10) {
			//ofLog() << "yolcudur abbas baglasan durmaz";
		}
	}


	virtual void onPress(int x, int y, int button) {
		//sFac = orgSFac;
		isDownStart = true;
		offSet = glm::vec2(x, y) - glm::vec2(this->x, this->y);
		touchStart = glm::vec2(x, y);

		//printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		Tweenzor::add(&sFac, sFac, orgSFac, 0.0, 0.25f, EASE_OUT_EXPO);
		
		ofNotifyEvent(ofxFilikaEvents::ON_INT_RETURN, btnId, this);
	}

	
	virtual void onGlobalMouseMove(int x, int y) {
		//ofLog() << "x:" << x << "y:" << y;
		
	}


	virtual void onDragOver(int x, int y, int button) {
		//ofLog() << "Dragging x:" << x << "y:" << y;
		
	}

	virtual void onRelease(int x, int y, int button) {
		//printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
		
		

		checkSwipe(glm::vec2(x, y), touchStart);

	}

	virtual void onReleaseOutside(int x, int y, int button) {
		//ofxFilikaButtonIMG::onReleaseOutside(x, y, button);

		//printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);

		checkSwipe(glm::vec2(x, y), touchStart);


	}

	void checkSwipe(glm::vec2 ts, glm::vec2 te) {
		touchEnd = ts;

		swipeSpd.x = touchEnd.x - touchStart.x;
		swipeSpd.y = touchEnd.y - touchStart.y;
		float speedG = ofDist(touchStart.x, touchStart.y, touchEnd.x, touchEnd.y);
		ofLog() << "horz speed: " << swipeSpd.x;
		ofLog() << "vert speed: " << swipeSpd.y;
		ofLog() << "median speed: " << ofDist(touchStart.x, touchStart.y, touchEnd.x, touchEnd.y);

		isDownStart = false;

		// SWIPE UP OCCURS
		if (speedG > globalMagnitudeThreshold && swipeSpd.y < verticalSpeedThreshold) {
			ofNotifyEvent(ON_SWIPE, this);
		}
		else {
			Tweenzor::add(&sFac, sFac, 0, 0.0, 0.25f, EASE_OUT_QUAD);
			ofNotifyEvent(ofxFilikaEvents::ON_RELEASE, this);
		}
	}

	void setSwipeUpSettings(float globalMagnitudeThreshold, float verticalSpeedThreshold) {
		this->globalMagnitudeThreshold = globalMagnitudeThreshold;
		this->verticalSpeedThreshold = verticalSpeedThreshold;
	}
};