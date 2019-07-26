//
//  ofxFilikaColorButton.h
//  drawingBoothv1
//
//  Created by alp tugan on 10/01/18.
//
//
#ifndef ofxFilikaDraggableButton_h
#define ofxFilikaDraggableButton_h

#include "ofMain.h"
//#include "ofxTweenzor.h"
#include "ofxFilikaConstants.h"





class ofxFilikaDraggableButton {
	

private:
    int saveX, saveY;
    int xpos, ypos, w, hitx, hity, bId, h;
    float scaleFac, _scaleMinVal, _scaleMaxVal, targetScale;
    bool isAnimatable, delay;
    int _w, _h;
    int _bgOpacity;
    bool isPassiveMode;
    bool isEnabledInteraction;
    string imgPath;
    string pivot;
    bool isDown;
    bool isDragging;
    bool isDraggingV;
    bool isDraggingH;
    
    // Tween Lib;
    float btnAnimT;
	ofxFilikaBgMode bgMode;
	ofxFilikaButtonMode buttonMode;
    ofVec2f bgSize;
    ofVec2f touchStartPos;
	ofVec2f interactionArea;
    int sbRoundness;
public:
	

	ofEvent<int> BUTTON_TOUCH_DOWN;
	ofEvent<int> BUTTON_TOUCH_UP;
    ofEvent<ofVec2f> BUTTON_DRAGGING;

	ofColor mainColor;
    ofColor _OVER_COLOR;
    ofColor _PRESS_COLOR;
    ofColor _OUT_COLOR;
	ofFbo fbo;
	bool result;
	bool isSelected;
	bool isMouseEnabled;
	bool isTouchEnabled;
    
	int strokeSize;
    
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
	void setTouchDownScaleFac(float _sMin, float _sMax) {
		_scaleMinVal = _sMin;
		_scaleMaxVal = _sMax;
	}

	void setRoundness(int _v) {
		sbRoundness = _v;
	}

    void setDraggingVertical(bool _v) {
        isDraggingV = _v;
        //isDraggingH = _h;
    }

	void setDraggingHorizontal(bool _h) {
		isDraggingH = _h;
	}

    void setButtonMode(ofxFilikaButtonMode _v) {
        buttonMode = _v;
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

	void setBackgroundShape(ofxFilikaBgMode _mode) {
		bgMode = _mode;
	}

	void setPassiveImage(string src) {
		imgePassive.load(src);
		imgePassive.resize(_w, _h);
	}

	void setPassive(bool _val) {
		isPassiveMode = _val;
	}

	void setColorReleased(ofColor _releasedColor) {
		_OUT_COLOR = _releasedColor;
	}

	void setColorOver(ofColor _overColor) {
		_OVER_COLOR = _overColor;
	}

	void setColorPressed(ofColor _pressedColor) {
		_PRESS_COLOR = _pressedColor;
	}

	void setPivot(string _pivot) {
        pivot = _pivot;
		if (_pivot == "center") {
			pivotPoint.x = getWidth() * 0.5;
			pivotPoint.y = getHeight() * 0.5;
        }else if (_pivot == "tl") {
            pivotPoint.x = 0;
            pivotPoint.y = 0;
        }
	}

	void setInteractionArea(ofVec2f _a) {
		interactionArea = _a;
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
	ofxFilikaDraggableButton() {
		bId = 0;
		isSelected = false;
		_bgOpacity = 255;
	}

	// deconstructer
	~ofxFilikaDraggableButton() {}

    ////////////////////////////////////////////////
    // SETUP
    ////////////////////////////////////////////////
    void setup(ofVec2f _size, int _id, ofColor _mainColor = ofColor(0), bool _isAnimatable = false) {
        
        _OUT_COLOR = _mainColor;
        _OVER_COLOR = ofColor(0, 100, 0, 255);
        _PRESS_COLOR = ofColor(110, 110, 110, 255);
        
        isPassiveMode = false;
        sbRoundness   = 5;
       
#ifdef TOUCH_ENABLE
        ofRegisterTouchEvents(this);
		isTouchEnabled = true;
#else
		// register events for interaction
		ofRegisterMouseEvents(this);
		isMouseEnabled = true;
#endif
        isEnabledInteraction = true;
        buttonMode = ofxFilikaButtonMode::BUTTON_MODE_SHAPE_ROUNRECT;
        
        mainColor = _mainColor;
        isAnimatable = _isAnimatable;
   
        bId = _id;
        targetScale = 1;
        scaleFac = 1;
		_scaleMinVal = 0.8;
		_scaleMaxVal = 1;
        
        btnAnimT = 0.125;
        bgMode = ofxFilikaBgMode::NONE;
        
        
        _w = _size.x;
        _h = _size.y;

		interactionArea.x = _w;
		interactionArea.y = _h;
        
    }
    
	////////////////////////////////////////////////
	// SETUP
	////////////////////////////////////////////////
	void setup(string _imgPath, int _id, ofVec2f _bgSize = ofVec2f(-1, -1), ofColor _mainColor = ofColor(0), bool _isAnimatable = true) {

		ofSetCircleResolution(64);

		isPassiveMode = false;

#ifdef TOUCH_ENABLE
		ofRegisterTouchEvents(this);
		isTouchEnabled = true;
#else
		// register events for interaction
		ofRegisterMouseEvents(this);
		isMouseEnabled = true;
#endif
		isEnabledInteraction = true;
        buttonMode = ofxFilikaButtonMode::BUTTON_MODE_IMAGE;

		mainColor = _mainColor;
		isAnimatable = _isAnimatable;

		bgSize = _bgSize;
		
		bId = _id;
		targetScale = 1;
		scaleFac = 1;
		_scaleMinVal = 0.8;
		_scaleMaxVal = 1;

		imgPath = _imgPath;

		btnAnimT = 0.125;
		bgMode = ofxFilikaBgMode::NONE;


		if(imgPath != "")
			imge.load(imgPath);

		if (bgSize.x > imge.getWidth()) {
			_w = bgSize.x;
			_h = bgSize.y;
		}
		else {
			_w = imge.getWidth();
			_h = imge.getHeight();
		}

		
			/*float aspect = imge.getWidth() / imge.getHeight();
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
			*/
		

		interactionArea.x = _w;
		interactionArea.y = _h;

		setPivot("center");
	}

	////////////////////////////////////////////////
	// SETTERS & GETTERS
	////////////////////////////////////////////////
	void draw(int _x, int _y) {
        
        
        xpos = _x + pivotPoint.x; // Set position according to pivot value. Default: 0,0
        ypos = _y + pivotPoint.y;
        
		
		ofPushMatrix();

		ofTranslate(xpos, ypos); // Move container to specified position

		if (isAnimatable) // Set container animatable or not animatable : Default: true
			scaleFac += (targetScale - scaleFac) * 0.8;

		ofScale(scaleFac, scaleFac); // Scale Value of the container

		if (bgMode == ofxFilikaBgMode::CUSTOM) { // Set background color and shape ROUNDED RECTANGLE
			ofPushMatrix();
			ofRotate(45);
			ofSetColor(mainColor);
			ofDrawRectRounded(-bgSize.x*0.5, -bgSize.y*0.5, bgSize.x, bgSize.y, 30);
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

		ofSetColor(255, 255); // Add passive mode image
        if(buttonMode == ofxFilikaButtonMode::BUTTON_MODE_IMAGE) {
            if (!isPassiveMode) {
				if (pivot == "center") {
					imge.draw(-imge.getWidth() * 0.5, -imge.getHeight() * 0.5);
				}
				else if (pivot == "tl") {
					imge.draw(0, 0);
				}
			}else{
                imgePassive.draw(-imgePassive.getWidth() * 0.5, -imgePassive.getHeight() * 0.5);
			}
        }else if(buttonMode == ofxFilikaButtonMode::BUTTON_MODE_SHAPE_ROUNRECT) {
            ofSetColor(mainColor);
            ofDrawRectRounded(ofRectangle(0,0,_w, _h), sbRoundness, sbRoundness, sbRoundness, sbRoundness);
        }
		
		ofPopMatrix();
	}
    
    void draw() {
        
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
		if (isDraggingH || isDraggingV)
			hitAndDrag(touch.x, touch.y);
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
		hitReleasedOutside();
	}
	//--------------------------------------------------------------
	void mouseMoved(ofMouseEventArgs & args) {
        
	}

	//--------------------------------------------------------------
	void mouseDragged(ofMouseEventArgs & args) {
        if(isDraggingH || isDraggingV)
            hitAndDrag(args.x, args.y);
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
    // CONTAINER DRAGGING HANDLER
    ////////////////////////////////////////////////
    void hitAndDrag(int _x, int _y) {
        if(isDown) {
            if(isDraggingV) {
                ypos = _y - saveY;
            }

			if (isDraggingH) {
				xpos = _x - saveX;
			}
			ofVec2f p = ofVec2f(xpos, ypos);
			ofNotifyEvent(BUTTON_DRAGGING, p);
        }
    }
    ////////////////////////////////////////////////
    // CONTAINER PRESSED HANDLER
    ////////////////////////////////////////////////
    void hitBegin(float touch_x, float touch_y) {
        if (hit(touch_x, touch_y)) {
            // Save for Dragging
            if(isDraggingV || isDraggingH) {
                saveX = touch_x - xpos;
                saveY = touch_y - ypos;
            }
            
            
            //if (isAnimatable)
            targetScale = _scaleMinVal;
            isDown = true;
            ofNotifyEvent(BUTTON_TOUCH_DOWN, bId);
            
            mainColor = _PRESS_COLOR;
            
            //cout << "isDown " << isDown << endl;
        }
    }
    
	////////////////////////////////////////////////
	// CONTAINER RELEASED HANDLER
	////////////////////////////////////////////////
	void hitReleased(float touch_x, float touch_y) {
		
		if (hit(touch_x, touch_y)) {
			
			if (isDown)
			{
				targetScale = _scaleMaxVal;
				ofNotifyEvent(BUTTON_TOUCH_UP, bId);
				isDown = false;
                
                mainColor = _OUT_COLOR;
                
				//cout << "isUp" << isDown << endl;
			}
			
		}
	}

	void hitReleasedOutside() {
		
		if (isDown)
		{
			targetScale = _scaleMaxVal;
			ofNotifyEvent(BUTTON_TOUCH_UP, bId);
			isDown = false;
			//cout << "outside " << isDown << endl;
            mainColor = _OUT_COLOR;
		}
	}

	////////////////////////////////////////////////
	// HIT DETECTION
	////////////////////////////////////////////////
	bool hit(int _x, int _y) {
        if(pivot == "center")
            result = (_x < xpos + interactionArea.x*0.5 && _x > xpos - interactionArea.x*0.5 && _y > ypos - interactionArea.y*0.5 && _y < ypos + interactionArea.y*0.5) ? true : false;
        
		if (pivot == "tl") {
			if (_w == interactionArea.x && _h == interactionArea.y) {
				result = (_x < xpos + interactionArea.x && _x > xpos && _y > ypos && _y < ypos + interactionArea.y) ? true : false;
			}
			
			if(_w < interactionArea.x || _h < interactionArea.y) {
				result = (_x < xpos + _w + (interactionArea.x - _w) * 0.5 && _x > xpos - (interactionArea.x - _w) * 0.5 && _y > ypos && _y < ypos + interactionArea.y) ? true : false;
			}
		}

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
