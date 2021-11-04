#pragma once

#include "ofMain.h"
#include "ofxFilikaUtils.h"
#include "ofxFilikaEvents.h"
#include "ofxTweenzor.h"

class ofxFilikaCounter : public ofxFilikaEvents {
public:
	void setup(int start, int stop, ofTrueTypeFont & _f) {
		startVal = start;
		endVal = stop;

		f = &_f;

		// must call this before adding any tweens //
		Tweenzor::init();

		
	}

	void update() {
		Tweenzor::update(ofGetElapsedTimeMillis());

		if (disableCounter) return;

		if (counterVal == -9999) {
			counterVal = startVal + 1;
		}


		if (startVal > endVal) {
			if (notifier.notifyPerSecond(1)) {
				counterVal -= 1;

				if (counterVal - 1 == endVal - 1) {
					// Reset Counter 
					counterVal = endVal;

					// Init flash effect, then trigger counter done event on complete
					Tweenzor::add(&alphaFlash, alphaFlash, 255, 0.f, 0.1f, EASE_OUT_EXPO);
					Tweenzor::addCompleteListener(Tweenzor::getTween(&alphaFlash), this, &ofxFilikaCounter::onCompleteFlash);
					
					// Disable counter
					disableCounter = true;
					
					ofLog() << "COUNTER is done. To restart call <startCounter() method>";
				}
			}
		}


	}

	void draw(int x, int y) {

		if (!disableCounter) {
			ofPushStyle();
			wt = f->getStringBoundingBox(ofToString(counterVal), 0, 0).getWidth();
			ht = f->getStringBoundingBox("X", 0, 0).getHeight();

			ofSetColor(0, 0, 0, 180);
			ofDrawCircle(x, y, ht + sin(ofGetElapsedTimef() * 6) * 10);

			ofSetColor(textColor);
			f->drawString(ofToString(counterVal), x - wt * 0.5, y + ht * 0.5 - 5);
			ofPopStyle();
		}
		ofPushStyle();
		ofSetColor(255, alphaFlash);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		ofPopStyle();
	}

	void startCounter() { disableCounter = false; counterVal = startVal + 1;}
	void stopCounter() { disableCounter = true; }
	//void restartCounter() {}
private:
	void onCompleteFlash(float* arg) {
		// remove event
		Tweenzor::removeCompleteListener(Tweenzor::getTween(&alphaFlash));

		Tweenzor::add(&alphaFlash, alphaFlash, 0, 0.f, 0.15f, EASE_IN_EXPO);
		Tweenzor::addCompleteListener(Tweenzor::getTween(&alphaFlash), this, &ofxFilikaCounter::onCompleteFlashEnd);

	}

	void onCompleteFlashEnd(float * arg) {

		Tweenzor::removeCompleteListener(Tweenzor::getTween(&alphaFlash));
		// Notify Event
		ofNotifyEvent(ON_COUNTER_DONE);

	}

	float alphaFlash = 0;

	int counterVal = -9999;
	int startVal, endVal;

	ofTrueTypeFont* f;
	int wt, ht;
	ofColor textColor = ofColor(255,255,255,200);
	int size;
	
	bool disableCounter = true;
	ofxFilikaUtils notifier;
};