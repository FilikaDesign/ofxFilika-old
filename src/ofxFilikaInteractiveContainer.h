#pragma once

#include "ofMain.h"

/* USAGE */
/* Create a template class and extend this class to ofxFilikaInteractiveContainer as follows */
/*
#include "ofMain.h"
#include "ofxFilikaInteractiveContainer.h"

class yourClassName : public ofxFilikaInteractiveContainer {
public:
	float sFac;

	void setup() { 
		// Override setup function
		enableMouseEvents();	// to enable mouse events
		enableTouchEvents();	// to enable touch events

		// you should set your display object's screen coordinates and size
		// otherwise interaction doesn't work.
		// Keep in mind that objects are aligned according to top-left corner
		// But, they are scaled and rotated from center of the container
		this->x = 100;
		this->y = 100;
		this->width = 200;
		this->height = 200;
	}

	void draw() {
		ofPushMatrix();
		ofTranslate(this->x + this->width * 0.5, this->y + this->height*0.5);
		ofScale(1 + sFac, 1 + sFac);
		ofTranslate(-this->width*0.5, -this->height*0.5);
		ofDrawRectangle(0,0, width, height);
		ofPopMatrix();

		// Override draw function
		drawDebug();			// to enable screen debug view of your container
	}

	virtual void onPress(int x, int y, int button) {
		//printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = -0.1;
	}

	virtual void onRelease(int x, int y, int button) {
		//printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = 0;
	}

	virtual void onReleaseOutside(int x, int y, int button) {
		//printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
		sFac = 0;
	}
}

*/

class ofxFilikaInteractiveContainer : public ofRectangle {
private:
	bool isMouseEventsEnabled;
	bool isTouchEventsEnabled;

	bool				   _isMouseOver;					 // is mouse over the rect
	std::map<int, bool>    _isMousePressed;					 // is mouse down over the rect (for any given mouse button)
	unsigned long		   _stateChangeTimestampMillis;
public:

	//--------------------------------------------------------------
	void enableInteraction() {
		if (!isTouchEventsEnabled || !isMouseEventsEnabled)
		{
			enableTouchEvents();
			enableMouseEvents();
			isTouchEventsEnabled = true;
			isMouseEventsEnabled = true;
		}
	}

	void disableInteraction() {
		if (isTouchEventsEnabled || isMouseEventsEnabled)
		{
			disableTouchEvents();
			disableMouseEvents();
			isTouchEventsEnabled = false;
			isMouseEventsEnabled = false;
		}
	}

	//--------------------------------------------------------------
	void enableMouseEvents() {
		ofAddListener(ofEvents().mousePressed, this, &ofxFilikaInteractiveContainer::_mousePressed);
		ofAddListener(ofEvents().mouseMoved, this, &ofxFilikaInteractiveContainer::_mouseMoved);
		ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaInteractiveContainer::_mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &ofxFilikaInteractiveContainer::_mouseReleased);
		isMouseEventsEnabled = true;
	}

	void disableMouseEvents() {
		ofRemoveListener(ofEvents().mousePressed, this, &ofxFilikaInteractiveContainer::_mousePressed);
		ofRemoveListener(ofEvents().mouseMoved, this, &ofxFilikaInteractiveContainer::_mouseMoved);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxFilikaInteractiveContainer::_mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &ofxFilikaInteractiveContainer::_mouseReleased);
		isMouseEventsEnabled = false;
	}


	//--------------------------------------------------------------
	void enableTouchEvents() {
		ofAddListener(ofEvents().touchDown, this, &ofxFilikaInteractiveContainer::_touchDown);
		ofAddListener(ofEvents().touchUp, this, &ofxFilikaInteractiveContainer::_touchUp);
		ofAddListener(ofEvents().touchMoved, this, &ofxFilikaInteractiveContainer::_touchMoved);
		isTouchEventsEnabled = true;
	}

	void disableTouchEvents() {
		ofRemoveListener(ofEvents().touchDown, this, &ofxFilikaInteractiveContainer::_touchDown);
		ofRemoveListener(ofEvents().touchUp, this, &ofxFilikaInteractiveContainer::_touchUp);
		ofRemoveListener(ofEvents().touchMoved, this, &ofxFilikaInteractiveContainer::_touchMoved);
		isTouchEventsEnabled = false;
	}

	void drawDebug() {
		ofPushMatrix();
		ofTranslate(this->x, this->y);
		ofPushStyle();
		string ss;
		if (isMousePressed()) {
			ofFill();
			ofSetColor(0, 255, 0, 100);
			ss = "pointer pressed on object";
		}
		else if (isMouseOver()) {
			ofFill();
			ofSetColor(255, 255, 0, 50);
			ss = "pointer over on object";
		}
		else {
			ofNoFill();
			ofSetColor(ofColor::red);
			ss = "pointer outside object";
		}

		ofDrawRectangle(0, 0, this->getWidth(), this->getHeight());
		ofDrawBitmapStringHighlight(ss, 3, 17);
		ofPopStyle();
		ofPopMatrix();
	}

	//--------------------------------------------------------------
	bool ofxFilikaInteractiveContainer::isMouseOver() const {
		return _isMouseOver;
	}

	//--------------------------------------------------------------
	bool ofxFilikaInteractiveContainer::isMousePressed(int mouseButton = 0) const {
		if (_isMousePressed.find(mouseButton) == _isMousePressed.end()) return false;
		return _isMousePressed.at(mouseButton);
	}

	//--------------------------------------------------------------
	ofxFilikaInteractiveContainer() {
		isTouchEventsEnabled = false;
		isMouseEventsEnabled = false;
		//enableMouseEvents();
	}


	//--------------------------------------------------------------
	virtual ~ofxFilikaInteractiveContainer() {
		disableTouchEvents();
		disableMouseEvents();
	}


	virtual bool hitTest(int tx, int ty) const		// returns true if given (x, y) coordinates (in screen space) are over the object (based on position and size)
	{
		return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
	}

	// extend ofxMSAInteractiveObject and override all or any of the following methods
	// called when app starts
	virtual void setup() {
	
	}     

	// called every frame to draw object
	virtual void draw() {
		drawDebug();
	}      
	

	// these behave very similar to those in flash
	virtual void onRollOver(int x, int y) {}                    // called when mouse enters object x, y, width, height
	virtual void onRollOut() {}                                 // called when mouse leaves object x, y, width, height
	virtual void onMouseMove(int x, int y) {}                   // called when mouse moves while over object x, y, width, height
	virtual void onDragOver(int x, int y, int button) {}        // called when mouse moves while over object and button is down
	virtual void onDragOutside(int x, int y, int button) {}     // called when mouse moves while outside the object after being clicked on it
	virtual void onPress(int x, int y, int button) {}           // called when mouse presses while over object
	virtual void onPressOutside(int x, int y, int button) {}    // called when mouse presses while outside object
	virtual void onRelease(int x, int y, int button) {}         // called when mouse releases while over object
	virtual void onReleaseOutside(int x, int y, int button) {}  // called when mouse releases outside of object after being pressed on object
	//virtual void onKeyPress(int key) {}                         // called when keypressed while mouse over the object
	//virtual void onKeyRelease(int key) {}                        // called when keyreleased while mouse over the object
	virtual void onGlobalMouseMove(int x, int y) {}
	//virtual void onDrag(int x, int y) {}

	private: 
		void _mouseMoved(ofMouseEventArgs &e) {
			int x = e.x;
			int y = e.y;
			int button = e.button;

			_pointerMoved(x, y, button);
		}


		//--------------------------------------------------------------
		void _mousePressed(ofMouseEventArgs &e) {
			int x = e.x;
			int y = e.y;
			int button = e.button;

			_pointerPressed(x, y, button);
		}

		//--------------------------------------------------------------
		void _mouseDragged(ofMouseEventArgs &e) {
			int x = e.x;
			int y = e.y;
			int button = e.button;

			_pointerDragged(x, y, button);
		}

		//--------------------------------------------------------------
		void _mouseReleased(ofMouseEventArgs &e) {
			int x = e.x;
			int y = e.y;
			int button = e.button;

			_pointerReleased(x, y, button);
		}

		void _touchDown(ofTouchEventArgs &e) {
			int x = e.x;
			int y = e.y;

			_pointerPressed(x, y, 0);
		}

		void _touchUp(ofTouchEventArgs &e) {
			int x = e.x;
			int y = e.y;

			_pointerReleased(x, y, 0);
			_isMouseOver = false;
		}

		void _touchMoved(ofTouchEventArgs &e) {
			int x = e.x;
			int y = e.y;

			_pointerDragged(x, y, 0);
		}

		void _pointerPressed(int x, int y, int bId) {
			if (!isMouseEventsEnabled) {
				_isMousePressed[bId] = false;
				return;
			}

			if (hitTest(x, y)) {								 // if mouse is over
				if (!isMousePressed(bId)) {						 // if wasn't down previous frame
					_isMousePressed[bId] = true;				 // update flag
					onPress(x, y, bId);					         // call onPress
				}
			}
			else {												// if mouse is not over
				_isMousePressed[bId] = false;					// update flag
				onPressOutside(x, y, bId);
			}

			_stateChangeTimestampMillis = ofGetElapsedTimeMillis();

			//mousePressed(x, y, button);
		}


		void _pointerReleased(int x, int y, int bId) {
			if (!isMouseEventsEnabled) {
				_isMousePressed[bId] = false;
				return;
			}

			if (hitTest(x, y)) {
				onRelease(x, y, bId);
			}
			else {
				if (isMousePressed(bId))
					onReleaseOutside(x, y, bId);
			}
			_isMousePressed[bId] = false;

			_stateChangeTimestampMillis = ofGetElapsedTimeMillis();

			//mouseReleased(x, y, button);
		}


		void _pointerMoved(int x, int y, int bId) {
			if (!isMouseEventsEnabled) return;

			if (hitTest(x, y)) {						// if mouse is over the object
				if (!_isMouseOver) {						// if wasn't over previous frame
					_isMouseOver = true;						// update flag
					onRollOver(x, y);						// call onRollOver
				}
				onMouseMove(x, y);						// and trigger onMouseMove
			}
			else if (_isMouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
				onRollOut();							// call onRollOut
				_isMouseOver = false;						// update flag
			}

			onGlobalMouseMove(x, y);
			_stateChangeTimestampMillis = ofGetElapsedTimeMillis();

			//mouseMoved(x, y);
		}

		void _pointerDragged(int x, int y, int bId) {
			if (!isMouseEventsEnabled) {
				_isMousePressed[bId] = false;
				return;
			}

			if (hitTest(x, y)) {							// if mouse is over the object
				if (!_isMouseOver) {						// if wasn't over previous frame
					//				onPress(x, y);							// call onPress - maybe not
					_isMouseOver = true;					// update flag
					onRollOver(x, y);						// call onRollOver
				}
				onDragOver(x, y, bId);					// and trigger onDragOver
			}
			else {
				if (_isMouseOver) {							// if mouse is not over the object, but the flag is true (From previous frame)
					onRollOut();							// call onRollOut
					_isMouseOver = false;					// update flag
				}
				if (isMousePressed(bId)) {
					onDragOutside(x, y, bId);
				}
				//_isMousePressed[button] = false;
			}

			_stateChangeTimestampMillis = ofGetElapsedTimeMillis();

			//mouseDragged(x, y, button);
		}
};