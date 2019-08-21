//
//  ofxFilikaContentScroller.h
//  example_ContentScroller
//
//  Created by alp tugan on 22.06.2019.
//

#ifndef ofxFilikaContentScroller_h
#define ofxFilikaContentScroller_h
#include "ofMain.h"
#include "ofxFilikaDraggableButton.h"

class ofxFilikaContentScroller : public ofRectangle {
private:
	ofBaseDraws * content;
	ofFbo mask;
	ofFbo contentFbo;

	ofRectangle scrollBarRect;
	ofRectangle scrollerRect;

	ofxFilikaDraggableButton scrollerBtn;
	ofxFilikaDraggableButton btnScrollUp;
	ofxFilikaDraggableButton btnScrollDown;

	glm::vec2 btnScrollUpPos;
	glm::vec2 btnScrollDownPos;

	bool isScrollBarVisible;
	bool isScrollingEnabled;

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
		moveContent(_p);
	}

public:
	void enableInteraction() {
		if (isScrollingEnabled) {
#ifdef TOUCH_ENABLE
			ofAddListener(ofEvents().touchMoved, this, &ofxFilikaContentScroller::touchMoveContentTouchHandler);
			ofAddListener(ofEvents().touchDown, this, &ofxFilikaContentScroller::touchDownContentTouchHandler);
			ofAddListener(ofEvents().touchUp, this, &ofxFilikaContentScroller::touchUpContentTouchHandler);
#else
			ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaContentScroller::moveMouseContent);
			ofAddListener(ofEvents().mousePressed, this, &ofxFilikaContentScroller::moveMouseContentPressed);
			ofAddListener(ofEvents().mouseReleased, this, &ofxFilikaContentScroller::moveMouseContentReleased);
#endif	
			if (isScrollBarVisible)
				ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);


			// Scroll Nav buttons enabled
			if (isScrollNavEnable) {
				ofAddListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentUp);
				ofAddListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentDown);
			}
		}
	}

	void disableInteraction() {
		if (isScrollingEnabled) {
#ifdef TOUCH_ENABLE
			ofRemoveListener(ofEvents().touchMoved, this, &ofxFilikaContentScroller::touchMoveContentTouchHandler);
			ofRemoveListener(ofEvents().touchDown, this, &ofxFilikaContentScroller::touchDownContentTouchHandler);
			ofRemoveListener(ofEvents().touchUp, this, &ofxFilikaContentScroller::touchUpContentTouchHandler);
#else
			ofRemoveListener(ofEvents().mousePressed, this, &ofxFilikaContentScroller::moveMouseContentPressed);
			ofRemoveListener(ofEvents().mouseDragged, this, &ofxFilikaContentScroller::moveMouseContent);
			ofRemoveListener(ofEvents().mouseReleased, this, &ofxFilikaContentScroller::moveMouseContentReleased);
#endif	
			if (isScrollBarVisible)
				ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);


			// Scroll Nav buttons enabled
			if (isScrollNavEnable) {
				ofRemoveListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentUp);
				ofRemoveListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentDown);
			}
		}
	}


	void setup(ofRectangle _contentRect, ofBaseDraws * _content = nullptr, int _sbGap = 1) {
		this->set(_contentRect);
		isPointerDown = false;

		sbGap = _sbGap;
		sbColor = ofColor(150, 150, 150, 60);
		scColor = ofColor(150, 150, 150, 255);
		sbW = 15;
		sbH = 0;
		scW = 15;
		scH = 0;
		moveAmty = 0;

		if (_content != nullptr)
			setContent(_content);

		enableInteraction();
	}

#ifdef TOUCH_ENABLE
	void touchMoveContentTouchHandler(ofTouchEventArgs  & e) {
		pointerMove(glm::vec2(e.x, e.y));
	}

	void touchDownContentTouchHandler(ofTouchEventArgs  & e) {
		pointerDown(glm::vec2(e.x, e.y));
	}

	void touchUpContentTouchHandler(ofTouchEventArgs & e) {
		pointerUp(glm::vec2(e.x, e.y));
	}

#else
	void moveMouseContentPressed(ofMouseEventArgs & e) {
		pointerDown(glm::vec2(e.x, e.y));
	}

	void moveMouseContent(ofMouseEventArgs & e) {
		pointerMove(glm::vec2(e.x, e.y));
	}

	void moveMouseContentReleased(ofMouseEventArgs & e) {
		pointerUp(glm::vec2(e.x, e.y));
	}
#endif	
	void pointerDown(glm::vec2 e) {
		if (this->inside(e)) {
			//saveX = e.x - scrollerBtn.getPos().x;
			saveY = ofMap(e.y, scrollBarRect.y, scrollBarRect.y + scrollBarRect.getHeight(), scrollBarRect.y + scrollBarRect.getHeight(), scrollBarRect.y) - scrollerRect.y;
			isPointerDown = true;
		}
	}

	void pointerUp(glm::vec2 e) {
		isPointerDown = false;
	}

	void pointerMove(glm::vec2 e) {
		if (isPointerDown && this->inside(e)) {
			
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
			moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, scrollBarRect.y, maxYVal, scrollBarRect.y, scrollBarRect.y + content->getHeight() - scrollBarRect.height + 20)*-1;
		else
			moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, maxYVal, scrollBarRect.y, scrollBarRect.y + content->getHeight() - scrollBarRect.height + 20, scrollBarRect.y)*-1;

		updateContentFbo();
	}

private:
	void updateContentFbo() {
		// Update & slide the content fbo
		contentFbo.begin();
		ofClear(0, 0);
		content->draw(0, moveAmty);
		contentFbo.end();
	}
public:
	void draw() {
		if (content != nullptr)
		{
			contentFbo.getTexture().setAlphaMask(mask.getTexture());
			//ofDrawRectangle(x, y, width, height );

			contentFbo.draw(x, y);

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
	}

	// SETTERTS & GETTERS
	void setContent(ofBaseDraws * _content) {
		content = _content;
		mask.clear();
		mask.allocate(content->getWidth(), content->getHeight(), GL_RGBA);
		mask.begin();
		ofClear(0, 0);
		ofDrawRectangle(0, 0, width, height);
		mask.end();

		contentFbo.clear();
		contentFbo.allocate(content->getWidth(), content->getHeight(), GL_RGBA);
		contentFbo.begin();
		ofClear(0, 0);
		content->draw(0, 0);
		contentFbo.end();

		// content height > Holder size ? enable scrollbar;
		if (content->getHeight() > height) {

			// Scroll Bar
			sbH = height;
			scrollBarRect.set(x + width + sbGap, y, sbW, sbH);

			// Scroller
			int diff = content->getHeight() - height;

			scH = MAX(height - diff, 100);
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

			if (isScrollBarVisible)
				ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);


			isScrollBarVisible = true;
			isScrollingEnabled = true;

			ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);
		}
		else {
			isScrollBarVisible = false;
			disableInteraction();
			isScrollingEnabled = false;
		}
	}

	
	void setPositon(int _x, int _y) {
		x = _x;
		y = _y;

		scrollBarRect.x = x + width + sbGap;
		scrollBarRect.y = y;

		scrollerRect.x = scrollBarRect.x;
	}

	void setContentRect(ofRectangle & _r) {
		this->set(_r);
	}

	void setScrollerGap(int  _v) {
		sbGap = _v;
	}

	void setScrollbarVisible(bool _v) {
		isScrollBarVisible = _v;
	}

	void setScrollingEnable(bool _isScrollingEnabled) {
		isScrollingEnabled = _isScrollingEnabled;

		if (!isScrollingEnabled)
			disableInteraction();
		else
			enableInteraction();
	}

	void setScrollbarW(int _v) {
		scW = _v;
		sbW = scW;
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
			ofAddListener(btnScrollUp.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentUp);
			ofAddListener(btnScrollDown.BUTTON_TOUCH_UP, this, &ofxFilikaContentScroller::moveContentDown);
		}
	}

	void setScrollNavPos(glm::vec2 _upPos, glm::vec2 _downPos) {
		btnScrollDownPos = _downPos;
		btnScrollUpPos = _upPos;
	}
};

#endif /* ofxFilikaContentScroller_h */
