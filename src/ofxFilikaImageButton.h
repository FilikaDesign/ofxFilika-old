//
//  ofxFilikaColorButton.h
//  drawingBoothv1
//
//  Created by alp tugan on 10/01/18.
//
//
#ifndef ofxFilikaImageButton_h
#define ofxFilikaImageButton_h

#include "ofMain.h"
#include "ofxFilikaConstants.h"
//#include "ofxTweenzor.h"



class ofxFilikaImageButton {
private:
	int xpos, ypos, w, hitx, hity, bId, h;
	int buttonMode;
	float scaleFac;
	bool isAnimatable, delay;
	int _w, _h;
	int _bgOpacity;
	bool isPassiveMode;
	bool isEnabledInteraction;
	string imgPath;
	bool isDown;

	// Tween Lib
	float x1, x2, targetScale;
	float btnAnimT;
	int bgMode;

	ofVec2f bgSize;
	ofVec2f touchStartPos;
	
	ofTrueTypeFont * textFont;
	string textStr;
	ofColor textColor;
	glm::vec2 textPos;
public:
	ofEvent<int> BUTTON_TOUCH_DOWN;
	ofEvent<int> BUTTON_TOUCH_UP;

	ofColor mainColor;
	ofFbo fbo;
	bool result;
	bool isSelected;
	bool isMouseEnabled;
	bool isTouchEnabled;
	int size;
	int strokeSize;
	int pivot;
	ofImage imge;
	ofImage imgePassive;
	ofVec2f pivotPoint;

	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	/*void setMouseEnable(bool _val) {
		isMouseEnabled = _val;
		if (_val) {
			ofRegisterMouseEvents(this);
		}else {
			ofUnregisterMouseEvents(this);
		}
			
	}*/
	void setOverlayFont(ofTrueTypeFont * _f, string _str, glm::vec2 _textPos, ofColor _textColor) {
		textFont = _f;
		textStr = _str;
		textColor = _textColor;
		textPos = _textPos;
	}

	void setOverlayText(string _str, ofxFilikaAlignment _alignment = TOP_LEFT, int _hTrFix = 0) {
		textStr = _str;

		if (_alignment == CENTER) {
			textPos.x = -textFont->getStringBoundingBox(_str, 0, 0).getWidth()*0.5;
			if(_hTrFix == 0)
				textPos.y = textFont->getStringBoundingBox(_str, 0, 0).getHeight()*0.5;
			else 
				textPos.y = textFont->getStringBoundingBox("W", 0, 0).getHeight()*0.5 + _hTrFix;
		}
	}

	void setOverlayTextPosition(glm::vec2 _textPos) {
		textPos = _textPos;
	}

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

	void setSelected(bool _val) {
		isSelected = _val;
	}

	void setAnimatable(bool _isAnimatable) {

		isAnimatable = _isAnimatable;
	}

	void setPos(int _x, int _y) {
		xpos = _x;
		ypos = _y;
	}

	void setBackgroundShape(int _mode = 0) {
		bgMode = _mode;
	}

	void setPassiveImage(string src) {
		imgePassive.load(src);
		imgePassive.resize(_w, _h);
	}

	void setPassive(bool _val) {
		isPassiveMode = _val;
	}

	void setPivot(string _pivot) {
		if (_pivot == "center") {
			pivotPoint.x = getWidth() * 0.5;
			pivotPoint.y = getHeight() * 0.5;
			pivot = ofxFilikaAlignment::CENTER;
		}
		else if (_pivot == "tl") {
			pivotPoint.x = 0;
			pivotPoint.y = 0;
			pivot = ofxFilikaAlignment::TOP_LEFT;
		}
	}

	bool getIsPassive() {
		return isPassiveMode;
	}

	void setBackGroundOpacity(int _val) {
		_bgOpacity = _val;
	}


	ofColor getColor() {
		return mainColor;
	}

	int getWidth() {
		return _w;
	}

	int getHeight() {
		return _h;
	}

	ofVec2f getPos() {
		return ofVec2f(xpos, ypos);
	}

	string getSourcePath() {
		return imgPath;
	}

	bool getIsEnabled() {
		return isEnabledInteraction;
	}

	////////////////////////////////////////////////
	// CONSTRUCTOR
	////////////////////////////////////////////////
	ofxFilikaImageButton() {
		bId = 0;
		isSelected = false;
		_bgOpacity = 255; 
		
	}

	// deconstructer
	~ofxFilikaImageButton() {
		disableInteraction();
	}



	////////////////////////////////////////////////
	// SETUP
	////////////////////////////////////////////////
	void setup(string _imgPath, int _size, int _id, ofxFilikaBgMode _bgMode = IMAGE, ofVec2f _bgSize = ofVec2f(-1, -1), ofColor _mainColor = ofColor(0), bool _isAnimatable = true) {
		isDown = false;
		ofSetCircleResolution(64);

		isPassiveMode = false;

//#ifdef TOUCH_ENABLE
		ofRegisterTouchEvents(this);
		isTouchEnabled = true;
//#else
		// register events for interaction
		ofRegisterMouseEvents(this);
		isMouseEnabled = true;
//#endif
		isEnabledInteraction = true;
		bgMode = _bgMode;
		mainColor = _mainColor;
		isAnimatable = _isAnimatable;
		size = _size;
		bgSize = _bgSize;
		strokeSize = _size + 10;
		bId = _id;
		x2 = size * 2.0;
		targetScale = 1;
		scaleFac = 1;

		if (_size == -1) {
			_w = bgSize.x;
			_h = bgSize.y;
		}

		scaleFac = 1;
		imgPath = _imgPath;

		btnAnimT = 0.125;
		bgMode = _bgMode;


		if (bgMode != IMAGE) {
			_w = bgSize.x;
			_h = bgSize.y;
		}

		if (imgPath != "") {
			imge.load(imgPath);
		}
			
		if (_size == -1) {
			if (imgPath != "")
			{
				_w = imge.getWidth();
				_h = imge.getHeight();
			}
		}

		if (size != -1 && imgPath != "")
		{
			float aspect = imge.getWidth() / imge.getHeight();
			float gap = 0.75;

			if (imge.getWidth() == max(imge.getWidth(), imge.getHeight())) {
				//landscape
				imge.resize(size * gap, (size / aspect) * gap);
				_w = size * gap;
				_h = (size / aspect) * gap;
			}
			else {
				//portrait
				imge.resize((size * aspect) * gap, size * gap);
				_w = (size * aspect) * gap;
				_h = size * gap;
			}
		}


		setPivot("center");
		pivot = ofxFilikaAlignment::CENTER;
	}

	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	void draw(int _x, int _y) {

		xpos = _x + pivotPoint.x; // Set position according to pivot value. Default: 0,0
		ypos = _y + pivotPoint.y;

		ofPushMatrix();
		ofPushStyle();
		ofTranslate(xpos, ypos); // Move container to specified position

		if (isAnimatable) // Set container animatable or not animatable : Default: true
			scaleFac += (targetScale - scaleFac) * 0.8;

		ofScale(scaleFac, scaleFac); // Scale Value of the container

		if (bgMode == ofxFilikaBgMode::CUSTOM) { // Set background color and shape ROUNDED RECTANGLE
			ofPushMatrix();
			ofRotate(45);
			ofSetColor(mainColor);
			ofDrawRectRounded(-size*0.5, -size*0.5, size, size, 30);
			ofPopMatrix();
		}
		else if (bgMode == ofxFilikaBgMode::RECTANGLE) { // Set background color and shape RECTANGLE
			ofPushMatrix();
			ofSetColor(mainColor);
			ofDrawRectangle(-bgSize.x*0.5, -bgSize.y*0.5, bgSize.x, bgSize.y);
			ofPopMatrix();
		}
		else if (bgMode == ofxFilikaBgMode::ELLIPSE) { // Set background color and shape ELLIPSE
			ofPushMatrix();
			ofSetColor(mainColor);
			ofDrawEllipse(0, 0, bgSize.x, bgSize.y);
			ofPopMatrix();
		}

		if (bgMode == ofxFilikaBgMode::IMAGE)
		{
			ofSetColor(255, 255); // Add passive mode image
			if (!isPassiveMode)
				imge.draw(-pivotPoint.x, -pivotPoint.y);
			else
				imgePassive.draw(-pivotPoint.x, -pivotPoint.y);

			if (isSelected) {
				ofDrawCircle(0, strokeSize - 10, 3);
			}
		}
		
		if (textStr != "") {
			ofPushStyle();
			ofSetColor(textColor);
			textFont->drawString(textStr, textPos.x, textPos.y);
			ofPopStyle();
		}
		ofPopStyle();
		ofPopMatrix();
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
		//hitReleased(touch.x, touch.y);
		hitReleasedOutside();
	}

	//--------------------------------------------------------------
	void touchDoubleTap(ofTouchEventArgs & touch) {
	}

	//--------------------------------------------------------------
	void touchCancelled(ofTouchEventArgs & touch) {
		hitReleasedOutside();
	}


	//--------------------------------------------------------------
	void mouseScrolled(ofMouseEventArgs & args) {

	}

	//--------------------------------------------------------------
	void mouseEntered(ofMouseEventArgs & args) {

	}

	//--------------------------------------------------------------
	void mouseExited(ofMouseEventArgs & args) {
		//hitReleasedOutside();
	}
	//--------------------------------------------------------------
	void mouseMoved(ofMouseEventArgs & args) {
		//hit(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mouseDragged(ofMouseEventArgs & args) {
		//hit(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mousePressed(ofMouseEventArgs & args) {
		hitBegin(args.x, args.y);
	}

	//--------------------------------------------------------------
	void mouseReleased(ofMouseEventArgs & args) {

		hitReleasedOutside();
	}

	////////////////////////////////////////////////
	// CONTAINER RELEASED HANDLER
	////////////////////////////////////////////////
	void hitReleased(float touch_x, float touch_y) {
		
		if (hit(touch_x, touch_y)) {
			
			if (isDown)
			{
				targetScale = 1.0;
				ofNotifyEvent(BUTTON_TOUCH_UP, bId);
				isDown = false;

				cout << "isUp" << isDown << endl;
			}
			
		}
	}

	////////////////////////////////////////////////
	// CONTAINER PRESSED HANDLER
	////////////////////////////////////////////////
	void hitBegin(float touch_x, float touch_y) {
		if (hit(touch_x, touch_y)) {

			//if (isAnimatable)
			targetScale = 0.8;
			isDown = true;
			ofNotifyEvent(BUTTON_TOUCH_DOWN, bId);

			cout << "isDown " << isDown << endl;
		}
	}

	void hitReleasedOutside() {
		
		if (isDown)
		{
			targetScale = 1.0;
			ofNotifyEvent(BUTTON_TOUCH_UP, bId);
			isDown = false;
			cout << "outside " << isDown << endl;
		}
	}

	////////////////////////////////////////////////
	// HIT DETECTION
	////////////////////////////////////////////////
	bool hit(int _x, int _y) {

		if(pivot == ofxFilikaAlignment::CENTER)
			result = (_x < xpos + _w*0.5 && _x > xpos - _w*0.5 && _y > ypos - _h*0.5 && _y < ypos + _h*0.5) ? true : false;

		else if(pivot == ofxFilikaAlignment::TOP_LEFT)
			result = (_x > xpos && _x < xpos + _w && _y > ypos && _y < ypos + _h) ? true : false;

		return result;
	}

	////////////////////////////////////////////////
	// DISABLE INTERACTION
	////////////////////////////////////////////////
	void disableInteraction() {
		if (isEnabledInteraction) {
			if(isMouseEnabled)
				ofUnregisterMouseEvents(this);
			
			if(isTouchEnabled)
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
};

#endif /* ofxFilikaImageButton_h */
