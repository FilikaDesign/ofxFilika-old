#pragma once
// TODO:: Move the following inputfield class to new file

#include "ofMain.h"

class ofxFilikaInputText {
public:
	ofRectangle r;
	glm::vec2 pos;
	string s,mf;
	ofTrueTypeFont *font;

	void setup(string _s, ofTrueTypeFont *_f, string _info) {
		font = _f;
		s = _s;
		mf = _info;

		pos = glm::vec2(0);
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(pos.x, pos.y + 50);
		ofPushStyle();
		ofSetColor(ofColor::white, 200);
		ofDrawRectRounded(r, 6);
		ofSetColor(ofColor::black, 255);

		// Info Text
		int mfw = font->getStringBoundingBox(mf, 0, 0).getWidth();
		int mfh = font->getStringBoundingBox(mf, 0, 0).getHeight();

		// In Text Draw
		ofSetColor(ofColor::black, 200);
		font->drawString(mf, r.width*0.5 - mfw*0.5, r.height*0.5 - mfh*0.5 - 30);
		ofSetColor(ofColor::white, 255);
		font->drawString(mf, r.width*0.5 - mfw*0.5 + 1, r.height*0.5 - mfh*0.5 - 30 + 1);

		int fw = font->getStringBoundingBox(s, 0, 0).width;
		int fh = font->getStringBoundingBox(s, 0, 0).height;

		// Input Text Draw
		ofSetColor(ofColor::black, 200);
		font->drawString(s, r.width*0.5 - fw*0.5, r.height*0.5 + fh*0.5);
		// blinking pointer
		ofSetColor(ofColor::dimGrey, sin(ofGetElapsedTimef() * 6) * 255);
		ofDrawRectangle(r.width*0.5 - fw*0.5 + fw + 2, -fh * 0.3, 2, 40);
		ofPopStyle();
		ofPopMatrix();
	}

	// SETTERS
	void setPos(glm::vec2 & _p) {
		pos = _p;
	}

	void setRect(ofRectangle & _r) {
		r = _r;
	}

	void setText(string _s) {
		s = _s;
	}

	void setInfoText(string _s) {
		mf = _s;
	}
};