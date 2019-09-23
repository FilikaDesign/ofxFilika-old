#pragma once
#include "ofMain.h"
class ofxFilikaTextButton :public ofRectangle {
public:
	ofEvent<int> BUTTON_CLICK;
	ofEvent<int> BUTTON_TOUCH;
	ofEvent<int> BUTTON_TOUCH_UP;

	int wt, ht;
	ofColor mainColor;
	string txt;
	int size;
	float scaleFac;
	bool isMouseEnabled;
	bool isTouchEnabled;
	bool result;
	bool isSelected;
	bool isEnabledInteraction;

	ofTrueTypeFont* f;
	ofTrueTypeFont* fBold;

	bool isDebug;
	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	void setTouchEnable(bool _val) {
		isTouchEnabled = _val;

		if (_val) {
			ofRegisterTouchEvents(this);
			if (isMouseEnabled) {
				isMouseEnabled = false;
				ofUnregisterMouseEvents(this);
			}
		}
		else {
			ofUnregisterTouchEvents(this);
			if (!isMouseEnabled) {
				isMouseEnabled = true;
				ofRegisterMouseEvents(this);
			}
		}
	}

	void setSelected(bool _val, ofColor _c) {
		isSelected = _val;
		if (_val) {
			mainColor = _c;
			//scaleFac = 1.2;
			scaleFac = 1;
		}
		else {
			scaleFac = 1;
			mainColor = _c;
		}
	}

	void setAnimatable(bool _isAnimatable, float _del = 0) {

		isAnimatable = _isAnimatable;
		delay = _del;

		if (isAnimatable) {
			/*params2.setup(&x1, x1, x2, _del, 0.4f, EASE_OUT_SINE);
			Tweenzor::add(params2);

			Tweenzor::addCompleteListener(Tweenzor::getTween(params2.property), this, &ofxFilikaColorButton::onCompleteAnim);*/
		}

	}

	void setAdditionalFont(ofTrueTypeFont * _f2) {
		fBold = _f2;
	}

	void setInteractionPadding(glm::vec2 _padding) {
		this->width += _padding.x;
		this->height += _padding.y;

	}

	ofColor getColor() {
		return mainColor;
	}

	int getWidth() {
		return this->width;
	}


	string getTxt() {
		return txt;
	}
	////////////////////////////////////////////////
	// CONSTRUCTOR
	////////////////////////////////////////////////
	ofxFilikaTextButton() {
		bId = 0;
		isSelected = false;
	}

	// deconstructer
	~ofxFilikaTextButton() {
		disableInteraction();
	}


	////////////////////////////////////////////////
	// SETUP
	////////////////////////////////////////////////
	void setup(ofTrueTypeFont & _f, string _txt, ofColor _mainColor, int _btnSize, int _id = 0, bool _isAnimatable = true) {
		isDebug = false;

		txt	  = _txt;
		
		f = &_f;

//#ifdef TOUCH_ENABLE
		ofRegisterTouchEvents(this);
		isTouchEnabled = true;
//#else
		// register events for interaction
		ofRegisterMouseEvents(this);
		isMouseEnabled = true;
//#endif
		mainColor = _mainColor;
		size = _btnSize;
		bId = _id;
		isAnimatable = _isAnimatable;
		scaleFac = 1.0;
		/*if(isAnimatable)
			Tweenzor::init();*/

		wt = f->getStringBoundingBox(txt, 0, 0).getWidth();
		ht = f->getStringBoundingBox("X", 0, 0).getHeight();

		this->set(0,0,wt,ht);
	}

	////////////////////////////////////////////////
	// HIT DETECTION
	////////////////////////////////////////////////
	bool hit(int _x, int _y) {

		//result = (_x < xpos + wt * 0.5 && _x > xpos - wt * 0.5 && _y > ypos - size * 0.5 && _y < ypos + size * 0.5) ? true : false;

		result = this->inside(glm::vec2(_x + this->width*0.5, _y + this->height*0.5));

		return result;
	}


	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	void draw(int _x, int _y) {
		xpos = _x;
		ypos = _y;

		this->x = xpos;
		this->y = ypos;

		ofPushMatrix();
		ofTranslate(this->x, this->y);

		ofScale(scaleFac, scaleFac);

		//ofScale(abs(sin(ofGetElapsedTimef())*2),abs(sin(ofGetElapsedTimef())*2));
		ofPushStyle();


		ofSetColor(mainColor);
		
		if (fBold == nullptr)
		{
			f->drawString(txt, -wt * 0.5, ht*0.5);
			if (isSelected) {
				ofScale((1 / scaleFac), (1 / scaleFac));
				ofDrawRectRounded(-(wt + 6) * 0.5, ht*0.5 + 5, (wt + 6), 2, 4);
			}
		}
		if (fBold != nullptr)
		{
			f->drawString(txt, -wt * 0.5, ht*0.5);

			if (isSelected) 
				fBold->drawString(txt, -wt * 0.5, ht*0.5);
		}
		
		ofPopStyle();
		ofPopMatrix();

		if (isDebug) {
			ofPushStyle();
			ofNoFill();
			ofSetColor(255, 0, 0);
			ofDrawRectangle(this->x - this->width * 0.5, this->y - this->height * 0.5, this->width, this->height);
			ofPopStyle();
		}
	}



	////////////////////////////////////////////////
	// INTERACTION
	////////////////////////////////////////////////
	//--------------------------------------------------------------
	void touchDown(ofTouchEventArgs & touch) {
		hitBegin(touch.x, touch.y);
	}

	//--------------------------------------------------------------
	void touchMoved(ofTouchEventArgs & touch) {
		
	}

	//--------------------------------------------------------------
	void touchUp(ofTouchEventArgs & touch) {
		hitReleasedOutside(touch.x, touch.y);
	}

	//--------------------------------------------------------------
	void touchDoubleTap(ofTouchEventArgs & touch) {
	}

	//--------------------------------------------------------------
	void touchCancelled(ofTouchEventArgs & touch) {
		result = false;
	}


	//--------------------------------------------------------------
	void mouseScrolled(ofMouseEventArgs & args) {

	}

	//--------------------------------------------------------------
	void mouseEntered(ofMouseEventArgs & args) {

	}

	//--------------------------------------------------------------
	void mouseExited(ofMouseEventArgs & args) {

	}
	//--------------------------------------------------------------
	void mouseMoved(ofMouseEventArgs & args) {
		hit(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mouseDragged(ofMouseEventArgs & args) {
		hit(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mousePressed(ofMouseEventArgs & args) {
		hitBegin(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mouseReleased(ofMouseEventArgs & args) {

		hitReleasedOutside(args.x, args.y);
	}

	////////////////////////////////////////////////
	// CONTAINER RELEASED HANDLER
	////////////////////////////////////////////////
	void hitReleased(float touch_x, float touch_y) {
		hit(touch_x, touch_y);
		if (result && isDown) {
			ofNotifyEvent(BUTTON_TOUCH_UP, bId);
			isDown = false;
		}

		if(!isSelected)
			scaleFac = 1.0;
	}

	void hitReleasedOutside(float touch_x, float touch_y) {
		hit(touch_x, touch_y);
		if (isDown) {
			ofNotifyEvent(BUTTON_TOUCH_UP, bId);
			isDown = false;
		}

		if (!isSelected)
			scaleFac = 1.0;
	}

	////////////////////////////////////////////////
	// CONTAINER PRESSED HANDLER
	////////////////////////////////////////////////
	void hitBegin(float touch_x, float touch_y) {
		hit(touch_x, touch_y);
		if (result) {

			if (!isSelected)
				scaleFac = 0.8;

			ofNotifyEvent(BUTTON_TOUCH, bId);

			isDown = true;
		}
	}

	////////////////////////////////////////////////
	// DISABLE INTERACTION
	////////////////////////////////////////////////
	void disableInteraction() {
		if (isEnabledInteraction) {
			if (isMouseEnabled)
				ofUnregisterMouseEvents(this);

			if (isTouchEnabled)
				ofUnregisterTouchEvents(this);
			isEnabledInteraction = false;
		}
	}

	////////////////////////////////////////////////
	// ENABLE INTERACTION
	////////////////////////////////////////////////
	void enableInteraction() {
		if (!isEnabledInteraction) {
			if (isMouseEnabled)
				ofRegisterMouseEvents(this);

			if (isTouchEnabled)
				ofRegisterTouchEvents(this);
			isEnabledInteraction = true;
		}

	}

private:
	int xpos, ypos, w, hitx, hity, bId, h;
	bool isAnimatable, delay, isDown;


	// Tween Lib
	float x1, x2;

	//TweenParams params2;
};