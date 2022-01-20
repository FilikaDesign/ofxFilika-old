#pragma once

#include "ofMain.h"
#include "ofxFilikaInteractiveContainer.h"
#include "ofxFilikaEvents.h"



class ofxFilikaButtonIMG : public ofxFilikaInteractiveContainer, public ofxFilikaEvents {
public:
	float sFac;
	ofImage btnTex;
	//ofxThreadedImageLoader * loader;

	ofxFilikaButtonIMG() {}
	//ofxFilikaButtonIMG(ofxFilikaButtonIMG&&) = default;
	//ofxFilikaButtonIMG(const ofxFilikaButtonIMG &) = default;


	//ofxFilikaButtonIMG(const ofxFilikaButtonIMG&) = default;
	//ofxFilikaButtonIMG& operator=(const ofxFilikaButtonIMG&) = default;

	~ofxFilikaButtonIMG() {
		//loader.stopThread();
	}

	void setup(string src) {
		// Override setup function
		enableMouseEvents();	// to enable mouse events
		enableTouchEvents();	// to enable touch events

		// Load button image texture
		btnTex.load(src);

		// you should set your display object's screen coordinates and size
		// otherwise interaction doesn't work.
		// Keep in mind that objects are aligned according to top-left corner
		// But, they are scaled and rotated from center of the container
		this->x = 0;
		this->y = 0;
		this->width = btnTex.getWidth();
		this->height = btnTex.getHeight();
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(this->x + this->width * 0.5, this->y + this->height*0.5);
		ofScale(1 + sFac, 1 + sFac);
		ofTranslate(-this->width*0.5, -this->height*0.5);
		// Draw texture to 0,0 to enable scale animation from center

		if(btnTex.isAllocated() && btnTex.getWidth() > 0) {
			btnTex.draw(0, 0, this->width, this->height);
		}
		ofPopMatrix();

		// Override draw function
		//drawDebug();			// to enable screen debug view of your container
	}

	virtual void onPress(int x, int y, int button) {
		//printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = -0.1;
		ofNotifyEvent(ON_PRESS);
	}

	virtual void onRelease(int x, int y, int button) {
		//printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = 0;
		ofNotifyEvent(ON_RELEASE);
	}

	virtual void onReleaseOutside(int x, int y, int button) {
		//printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = 0;

		ofNotifyEvent(ON_RELEASE);
	}
};