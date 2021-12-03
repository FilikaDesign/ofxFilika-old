#pragma once
#include "ofMain.h"
#include "ofxFilikaInteractiveContainer.h"
#include "ofxFilikaUtils.h"

class ofxFilikaKeys:public ofxFilikaInteractiveContainer {
public:
	ofEvent<string> KEY_PRESSED;

	string getKey() {
		return key;
	}
	void setActive(bool state) {
		active = state;
	}
	void setLowercase() {
		key = keyLower;
		if (key == "assets/shift_close.png") {
			img.load(key);
		}
	}
	void setUppercase() {
		key = keyUpper;
		if (key == "assets/shift_open.png") {
			img.load(key);
		}
	}
	void setRoundness(float roundness) {
		this->roundness = roundness;
	}
	void setColorButton(ofColor colorButton) {
		this->colorButton = colorButton;
	}
	void setColorButtonPressed(ofColor colorButtonPressed) {
		this->colorButtonPressed = colorButtonPressed;
	}
	void setColorText(ofColor colorText) {
		this->colorText = colorText;
	}
	void setColorBase(ofColor colorBase) {
		this->colorBase = colorBase;
	}

	void setup(string keyLower, string keyUpper, ofTrueTypeFont &font, int x, int y, int w, int h) {
		enableMouseEvents();	// to enable mouse events
		enableTouchEvents();	// to enable touch events
		this->font = font;
		this->keyLower = keyLower;
		this->keyUpper = keyUpper;
		this->key = keyLower;
		this->active = true;
		this->x = x;
		this->y = y;
		this->width = w;
		this->height = h;
		this->roundness = 0;
		this->colorButton = ofColor(27, 36, 40);
		this->colorText = ofColor(230, 230, 230);
		this->colorButtonPressed = ofColor(73, 80, 83);
		this->colorBase = ofColor(54, 71, 79);
		this->sFac = 0;
		ext = utils.getFileExtension(key);
		if (ext != "") {
			img.load(key);
		}
	}
	void draw() {
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(x + width * 0.5f, y + height*0.5f);
		ofTranslate(-width*0.5, -height*0.5f);
		float currentRoundness;
		if (sFac < 0.5f) {
			ofSetColor(colorButton);
			currentRoundness = roundness;
		}
		else {
			ofSetColor(colorButtonPressed);
			currentRoundness = max(roundness, height*0.1f);
		}
		float affectedSFac = sFac;
		if (ext != "" || key == " ") {
			affectedSFac = 0;
		}
		ofDrawRectRounded(0, 0 - affectedSFac * height, width, height + height* affectedSFac, currentRoundness);
		ofSetColor(colorText);
		int fH = font.stringHeight("O");
		if (ext != "") {
			float imgH = fH;
			float imgW = imgH * img.getWidth() / img.getHeight();
			img.draw(width*0.5f-imgW*0.5f,height*0.5f-imgH*0.5f,imgW,imgH);
		}else {
			int fW = font.stringWidth(key);
			font.drawString(key, width * 0.5f - fW * 0.5f, height * 0.5f + fH *0.5f - affectedSFac * height);
		}
		if (!active) {
			ofSetColor(colorBase,150);
			ofDrawRectRounded(0, 0, width, height, currentRoundness);
		}
		ofPopMatrix();
		ofPopStyle();

		// Override draw function
		//drawDebug();			// to enable screen debug view of your container
	}

	virtual void onPress(int x, int y, int button) {
		if (active) {
			sFac = 1;
			ofNotifyEvent(KEY_PRESSED, key, this);
		}
	}
	virtual void onRelease(int x, int y, int button) {
		if (active) {
			sFac = 0;
			//ofNotifyEvent(KEY_PRESSED, key, this);
		}
	}
	virtual void onDragOver(int x, int y, int button) {
		if (active) {
			sFac = 1;
		}
	}
	virtual void onRollOut() {
		if (active) {
			sFac = 0;
		}
	}

private:
	bool active;
	string key, keyLower, keyUpper, ext;
	float roundness, sFac;
	ofColor colorButton, colorText, colorButtonPressed, colorBase;
	ofTrueTypeFont font;
	ofxFilikaUtils utils;
	ofImage img;
};