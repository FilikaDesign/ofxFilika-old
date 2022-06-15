#pragma once
#include "ofMain.h"
#include "ofxTweenzor.h"

class ziraatCallToActionAnim {
private:
	vector<ofImage>				slideBgAnimImgs;
	vector<float>				slideBgAnimVals;

	vector<TweenParams>			slideAnimTween;
	bool isButtonDown = false;
	string animBg = "arrowUp.png";
	int sx, sy;
	float gap;

public:
	void setup(string imgSrc = "arrowUp.png", int x = 0, int y = 0, int numItems = 10, float gap = 0.9) {
		
		// DO NOT INIT HERE->PARENT CLASS
		//Tweenzor::init();			// Init Tweenzor
		animBg = imgSrc;
		sx = x;
		sy = y;
		this->gap = gap;
		/* Background swipe animation */
		slideBgAnimImgs.resize(numItems);							// Use 10 arrow images to create animation	
		slideBgAnimVals.resize(slideBgAnimImgs.size());		// Init tween values vector
		slideAnimTween.resize(slideBgAnimImgs.size());		// Tween parameters vector

		for (int i = 0; i < slideBgAnimImgs.size(); i++)
		{
			slideBgAnimImgs[i].load(animBg);				// Load animation images
			slideBgAnimVals[i] = 0;							// Tween parameters to "0"
		}
	}

	void draw() {
		// Update tweenzor in the parent main class not here
		// Tweenzor::update(ofGetElapsedTimeMillis());


		// The following lines set the fadeout through top
		//-i * 40

		// Animating directional images
		for (int i = 0; i < slideBgAnimImgs.size(); i++)
		{
			ofPushStyle();
			ofSetColor(255, slideBgAnimVals[i] - i * 40);
			slideBgAnimImgs[i].draw(sx - slideBgAnimImgs[0].getWidth() * 0.5,
				sy - (slideBgAnimImgs[0].getHeight()*gap)*i);
			ofPopStyle();
		}
	}

	void startAnimation() {
		isButtonDown = true;
		for (int i = 0; i < slideBgAnimImgs.size(); i++)
		{
			slideAnimTween[i].setup(&slideBgAnimVals[i], 0, 255, 0.03f*i, 0.45f, EASE_OUT_EXPO);
			Tweenzor::add(slideAnimTween[i]);
		}

		Tweenzor::addCompleteListener(Tweenzor::getTween(&slideBgAnimVals[0]), this, &ziraatCallToActionAnim::onCompleteStartAnim);
	}
	void stopAnimation() {
		isButtonDown = false;

		// Content selected, show content 
		// Notify event
		for (int i = 0; i < slideBgAnimImgs.size(); i++)
		{
			slideAnimTween[i].setup(&slideBgAnimVals[i], 255, 0, 0.005f*i, 0.25f, EASE_OUT_EXPO);
			Tweenzor::add(slideAnimTween[i]);
		}

		Tweenzor::addCompleteListener(Tweenzor::getTween(&slideBgAnimVals[0]), this, &ziraatCallToActionAnim::onCompleteBtnSelectedAnimation);
	}

private:
	void onCompleteBtnSelectedAnimation(float * arg) {
		// reset alpha values to 0
		for (int i = 0; i < slideBgAnimImgs.size(); i++)
		{
			slideBgAnimVals[i] = 0;
		}

		//ofNotifyEvent(SCENE2_CONTENT_SELECTED, btnId);
		//disableInteraction();
	}

	void onCompleteStartAnim(float * arg) {
		for (int i = 0; i < slideBgAnimImgs.size(); i++)
		{
			slideAnimTween[i].setup(&slideBgAnimVals[i], 255, 0, 0.03f*i, 0.45f, EASE_OUT_EXPO);
			Tweenzor::add(slideAnimTween[i]);
		}
		if (isButtonDown)
			Tweenzor::addCompleteListener(Tweenzor::getTween(&slideBgAnimVals[slideBgAnimVals.size() - 1]), this, &ziraatCallToActionAnim::onStackComplete);
	}
	void onStackComplete(float * arg) {
		startAnimation();
	}

};