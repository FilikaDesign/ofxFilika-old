//
//  ofxFilikaContentScroller.h
//  example_ContentScroller
//
//  Created by alp tugan on 22.06.2019.
//

#ifndef ofxFilikaScroller_h
#define ofxFilikaScroller_h
#include "ofMain.h"
#include "ofxFilikaDraggableButton.h"

class ofxFilikaScroller {
private:
	
	bool isScrollingEnabled;

	ofRectangle scrollBarRect;
	ofRectangle scrollerRect;

	ofRectangle scrollerContentRect;

	ofRectangle scrollerMaskRect;

	ofxFilikaDraggableButton scrollerBtn;
	ofxFilikaDraggableButton btnScrollUp;
	ofxFilikaDraggableButton btnScrollDown;

	glm::vec2 btnScrollUpPos;
	glm::vec2 btnScrollDownPos;

	bool isScrollBarVisible;

	int saveX, saveY, touchYScrollerPos;
	int sbW;
	int sbH;

	int scW;
	int scH;

	int sbRoundness;

	int moveAmty;
	int sbGap;
	ofColor sbColor;
	ofColor scColor, scOverColor, scPressedColor;

	bool isScrollNavEnable;
	bool isPointerDown;

	void isDraggingHandler(ofVec2f & _p) {
		if(isScrollBarVisible)
			moveContent(_p);
	}

public:
	ofEvent<int> CONTENT_MOVED;

	void enableInteraction() {
		if (isScrollingEnabled) {
			ofAddListener(ofEvents().touchMoved, this, &ofxFilikaScroller::touchMoveContentTouchHandler);
			ofAddListener(ofEvents().touchDown, this, &ofxFilikaScroller::touchDownContentTouchHandler);
			ofAddListener(ofEvents().touchUp, this, &ofxFilikaScroller::touchUpContentTouchHandler);
			ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaScroller::moveMouseContent);
			ofAddListener(ofEvents().mousePressed, this, &ofxFilikaScroller::moveMouseContentPressed);
			ofAddListener(ofEvents().mouseReleased, this, &ofxFilikaScroller::moveMouseContentReleased);
		
			if (isScrollBarVisible)
				ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaScroller::isDraggingHandler);


			// Scroll Nav buttons enabled
			if (isScrollNavEnable) {
				ofAddListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaScroller::moveContentUp);
				ofAddListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaScroller::moveContentDown);
			}
		}
	}

	void disableInteraction() {
		if (isScrollingEnabled) {

			ofRemoveListener(ofEvents().touchMoved, this, &ofxFilikaScroller::touchMoveContentTouchHandler);
			ofRemoveListener(ofEvents().touchDown, this, &ofxFilikaScroller::touchDownContentTouchHandler);
			ofRemoveListener(ofEvents().touchUp, this, &ofxFilikaScroller::touchUpContentTouchHandler);
			ofRemoveListener(ofEvents().mousePressed, this, &ofxFilikaScroller::moveMouseContentPressed);
			ofRemoveListener(ofEvents().mouseDragged, this, &ofxFilikaScroller::moveMouseContent);
			ofRemoveListener(ofEvents().mouseReleased, this, &ofxFilikaScroller::moveMouseContentReleased);
			if (isScrollBarVisible)
				ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaScroller::isDraggingHandler);


			// Scroll Nav buttons enabled
			if (isScrollNavEnable) {
				ofRemoveListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaScroller::moveContentUp);
				ofRemoveListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaScroller::moveContentDown);
			}
		}
	}


	void setup(ofRectangle _contentRect, ofRectangle _scrollerMaskRect, int _sbGap = 1) {
		scrollerContentRect.set(_contentRect);
		scrollerMaskRect.set(_scrollerMaskRect);
		isPointerDown = false;

		sbGap = _sbGap;
		sbColor = ofColor(150, 150, 150, 60);
		scColor = ofColor(150, 150, 150, 255);
		sbW = 15;
		sbH = 0;
		scW = 15;
		scH = 0;
		moveAmty = 0;

	
		//setContent(_content);

		enableInteraction();
		ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaScroller::isDraggingHandler);
	}

	void touchMoveContentTouchHandler(ofTouchEventArgs  & e) {
		pointerMove(glm::vec2(e.x, e.y));
	}

	void touchDownContentTouchHandler(ofTouchEventArgs  & e) {
		pointerDown(glm::vec2(e.x, e.y));
	}

	void touchUpContentTouchHandler(ofTouchEventArgs & e) {
		pointerUp(glm::vec2(e.x, e.y));
	}

	void moveMouseContentPressed(ofMouseEventArgs & e) {
		pointerDown(glm::vec2(e.x, e.y));
	}

	void moveMouseContent(ofMouseEventArgs & e) {
		pointerMove(glm::vec2(e.x, e.y));
	}

	void moveMouseContentReleased(ofMouseEventArgs & e) {
		pointerUp(glm::vec2(e.x, e.y));
	}
	void pointerDown(glm::vec2 e) {
		if (scrollerMaskRect.inside(e)) {
			//saveX = e.x - scrollerBtn.getPos().x;
			saveY = ofMap(e.y, scrollBarRect.y, scrollBarRect.y + scrollBarRect.getHeight(), scrollBarRect.y + scrollBarRect.getHeight(), scrollBarRect.y) - scrollerRect.y;
			isPointerDown = true;
		}
	}

	void pointerUp(glm::vec2 e) {
		isPointerDown = false;
	}

	void pointerMove(glm::vec2 e) {
		if (isPointerDown && scrollerMaskRect.inside(e)) {
			
			moveContent(glm::vec2(0, ofMap(e.y, scrollBarRect.y, scrollBarRect.y + scrollBarRect.getHeight(), scrollBarRect.y + scrollBarRect.getHeight(), scrollBarRect.y) - saveY), "natural");
		}
	}

	void moveContentDown(int & _id) {
		moveContent(glm::vec2(0, scrollerRect.y + 15));
		updateContentFbo();
	}

	void moveContentUp(int & _id) {
		moveContent(glm::vec2(0, scrollerRect.y - 15));
		updateContentFbo();
	}

	void moveContent(glm::vec2 _p, string _dir = "default") {

		int maxYVal = scrollBarRect.y + scrollBarRect.height - scrollerBtn.getHeight();

		// Moves scroll button
		scrollerRect.y = _p.y;


		// Stop if the content reaches to top border
		if (_p.y < scrollBarRect.y) {
			scrollerRect.y = scrollBarRect.y;
		}

		// Stop, if the content reaches to end
		if (_p.y > maxYVal) {
			scrollerRect.y = maxYVal;
		}

		// Change moving direction
		if (_dir == "default")
			moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, scrollBarRect.y, maxYVal, scrollBarRect.y, scrollBarRect.y + scrollerContentRect.getHeight() - scrollBarRect.height)*-1;
		else
			moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, scrollBarRect.y + scrollBarRect.height, scrollBarRect.y, scrollBarRect.y + scrollerContentRect.getHeight(), scrollBarRect.y)*-1;

		updateContentFbo();
	}

private:
	void updateContentFbo() {
		ofNotifyEvent(CONTENT_MOVED, moveAmty, this);
	}
public:
	void draw() {
		

			if (isScrollBarVisible) {
				ofPushStyle();

				// Scroll Bar

				
				ofSetColor(sbColor);
				ofDrawRectRounded(scrollBarRect, sbRoundness, sbRoundness, sbRoundness, sbRoundness);

				// Scroller
				//ofSetColor(scColor);
				//ofDrawRectRounded(scrollerRect, sbRoundness, sbRoundness, sbRoundness, sbRoundness);
				scrollerBtn.draw(scrollerRect.x, scrollerRect.y);

				if (isScrollNavEnable) {
					btnScrollUp.draw(btnScrollUpPos.x, btnScrollUpPos.y);
					btnScrollDown.draw(btnScrollDownPos.x, btnScrollDownPos.y);
				}

				ofPopStyle();

			}
		
	}

	// SETTERTS & GETTERS
	void setContent(ofRectangle _content, ofRectangle _scrollerMaskRect) {

		scrollerContentRect.set(_content);
		scrollerMaskRect.set(_scrollerMaskRect);

		// content height > Holder size ? enable scrollbar;
		if (scrollerContentRect.getHeight() > scrollerMaskRect.getHeight()) {

			// Scroll Bar
			sbH = scrollerMaskRect.getHeight();
			scrollBarRect.set(scrollerMaskRect.x + scrollerMaskRect.getWidth() + sbGap, scrollerMaskRect.y, sbW, sbH);

			// Scroller
			int diff = scrollerContentRect.getHeight() - scrollerMaskRect.getHeight();

			scH = MAX(scrollerMaskRect.getHeight() - diff, 100);
			scrollerRect.set(scrollBarRect.x, scrollBarRect.y, scW, scH);

			// ofVec2f _size, int _id, ofColor _mainColor = ofColor(0), bool _isAnimatable = false
			scrollerBtn.setup(ofVec2f(scrollerRect.width, scrollerRect.height), 0, scColor);
			scrollerBtn.setColorReleased(scColor);
			scrollerBtn.setColorPressed(scPressedColor);
			scrollerBtn.setColorOver(scOverColor);
			scrollerBtn.setPivot("tl");
			scrollerBtn.setRoundness(sbRoundness);
			scrollerBtn.setDraggingVertical(true);
			scrollerBtn.setInteractionArea(ofVec2f(scrollerRect.width + 40, scrollerRect.height));

			isScrollBarVisible = true;
			isScrollingEnabled = true;
			
		}
		else {
			isScrollBarVisible = false;
			disableInteraction();
			isScrollingEnabled = false;
		}
	}


	void setScrollingEnable(bool _isScrollingEnabled) {

		isScrollingEnabled = _isScrollingEnabled;
		if (!isScrollingEnabled)
			disableInteraction();
		else
			enableInteraction();

	}




	void setPositionScrobber(int _y) {
		scrollerRect.y = scrollerContentRect.y;
	}

	void setPositionScrobberX(int _x) {
		scrollerRect.x = scrollBarRect.x + _x;
	}

	void setPositon(int _x, int _y) {
		scrollerMaskRect.x = _x;
		scrollerMaskRect.y = _y;
		scrollerContentRect.x = _x;
		scrollerContentRect.y = _y;
		
		scrollBarRect.x = scrollerMaskRect.x + scrollerMaskRect.getWidth() + sbGap;
		scrollBarRect.y = scrollerMaskRect.y;

		scrollerRect.x = scrollBarRect.x ;
	}

	void setContentRect(ofRectangle & _r) {
		//this->set(_r);
	}

	void setScrollerGap(int  _v) {
		sbGap = _v;
	}

	void setScrollbarVisible(bool _v) {
		isScrollBarVisible = _v;
	}

	void setScrollbarW(int _v) {
		scW = _v;
		sbW = scW;
	}

	void setScrollerW(int _w) {
		scW = _w;
	}

	void setScrollbarCornerRadius(int _v) {
		sbRoundness = _v;
	}

	void setScrollbarColor(ofColor _v) {
		sbColor = _v;
	}

	void setScrollerColor(ofColor _idleColor, ofColor _pressedColor, ofColor _overColor) {
		scColor = _idleColor;
		scPressedColor = _pressedColor;
		scOverColor = _overColor;
	}

	/* SCROLL NAVIGATION */
	void setScrollNavEnable(bool _isEnable) {
		isScrollNavEnable = _isEnable;
	}

	void setScrollNavigation(string _btnUpSrc, string _btnDownSrc, bool _enable = true) {
		isScrollNavEnable = _enable;
		btnScrollDown.setup(_btnDownSrc, 0);
		btnScrollUp.setup(_btnUpSrc, 0);

		if (isScrollNavEnable) {
			ofAddListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaScroller::moveContentUp);
			ofAddListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaScroller::moveContentDown);
		}
	}

	void setScrollNavPos(glm::vec2 _upPos, glm::vec2 _downPos) {
		btnScrollDownPos = _downPos;
		btnScrollUpPos = _upPos;
	}

	int getMoveAmtYValue() {
		return moveAmty;
	}
};

#endif /* ofxFilikaContentScroller_h */
