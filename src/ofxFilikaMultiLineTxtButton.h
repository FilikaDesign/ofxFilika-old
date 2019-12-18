#pragma once
#include "ofMain.h"
#include "ofxFilikaInteractiveContainer.h"
#include "ofxTweenzor.h"
#include "ofxTextSuite.h"

class ofxFilikaMultiLineTxtButton : public ofxFilikaInteractiveContainer {
private:
	ofxTextBlock				content;
	ofColor						color;
	int direction;

public:
	float sFac;
	ofEvent<int> TEXTBUTTON_CLICKED;

	void setup(ofTrueTypeFont _bold30, int _w, ofColor _color, int _id = 0) {
		// Override setup function
		//enableMouseEvents();	// to enable mouse events
		//enableTouchEvents();	// to enable touch events

		color = _color;

		content.init(&_bold30);								// Set default font for the content text box
		content.setLineHeight(32);
		content.wrapTextX(width);							// Wrap textbox width to 900



		// you should set your display object's screen coordinates and size
		// otherwise interaction doesn't work.
		// Keep in mind that objects are aligned according to top-left corner
		// But, they are scaled and rotated from center of the container
		this->x = 0;
		this->y = 0;
		this->width = content.getWidth();
		this->height = 200;
	}

	void setContent(string _str, int _w) {
		content.setText(_str);
		content.wrapTextX(_w);

		this->width = content.getWidth();
		this->height = content.getHeight();
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(this->x + this->width * 0.5, this->y + this->height*0.5);
		ofScale(1 + sFac, 1 + sFac);
		ofTranslate(-this->width*0.5, -this->height*0.5);
		ofDrawRectangle(0, 0, width, height);

		// Content Text
		ofPushStyle();
		content.setColor(color.r, color.g, color.b, color.a);
		content.drawLeft(0,0);
		ofPopStyle();

		ofPopMatrix();

		// Override draw function
		//drawDebug();			// to enable screen debug view of your container
	}

	virtual void onPress(int x, int y, int button) {
		//printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = -0.1;
		direction = 1;
		ofNotifyEvent(TEXTBUTTON_CLICKED, direction);
	}

	virtual void onRelease(int x, int y, int button) {
		//printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = 0;
		direction = -1;
		ofNotifyEvent(TEXTBUTTON_CLICKED, direction);
	}

	virtual void onReleaseOutside(int x, int y, int button) {
		//printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = 0;
		direction = -1;
		ofNotifyEvent(TEXTBUTTON_CLICKED, direction);
	}
};