#pragma once
#include "ofMain.h"
#include "ofxFilikaInteractiveContainer.h"

class ofxFilikaKeyboard_InputField : public ofxFilikaInteractiveContainer {
public:
	ofEvent<void> INPUT_FIELD_CLICKED;
	void setArea(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	void setActive(bool state) {
		active = state;
	}
	void setDrawingText(string drawingText) {
		this->drawingText = drawingText;
	}
	void setPadding(float padding) {
		this->padding = padding;
	}
	void setup(int x, int y, int width, int height, int padding, ofTrueTypeFont &font, ofColor colorText) {
		enableMouseEvents();	// to enable mouse events
		enableTouchEvents();	// to enable touch events
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->padding = padding;
		this->font = font;
		this->colorText = colorText;
		this->drawingText = "E-posta adresinizi giriniz. Göndermek için GÖNDER'e basınız.";
		this->active = false;
	}
	void draw() {
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(this->x, this->y);
		//ofSetColor(255,0,0);
		//ofDrawRectangle(0, 0, width, height);
		ofSetColor(colorText);

		padding = this->width * 0.5 - font.getStringBoundingBox(drawingText,0,0).getWidth() * 0.5;

		font.drawString(drawingText, padding,
			height * 0.5f + font.stringHeight("O")*0.5f);
		if (active) {
			ofSetColor(colorText, 255 * sin(ofDegToRad(ofGetElapsedTimeMillis()*0.36f)));
			ofDrawRectangle(padding + font.stringWidth(drawingText) + font.stringWidth("O")*0.15f,
				height * 0.5f - font.stringHeight("O")*0.8f,
				font.stringWidth("O")*0.1f, font.stringHeight("O")*1.6f);
		}
		ofPopMatrix();
		ofPopStyle();
	}

	virtual void onPress(int x, int y, int button) {
		//printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		active = true;
		ofNotifyEvent(INPUT_FIELD_CLICKED);
	}

	virtual void onRelease(int x, int y, int button) {
		//printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
		
	}

	virtual void onReleaseOutside(int x, int y, int button) {
		//printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
		
	}
private:
	float padding;
	ofColor colorText;
	string drawingText;
	bool active;
	ofTrueTypeFont font;
};