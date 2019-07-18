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
    
    bool isScrollBarVisible;
    
	int saveX, saveY;
    int sbW;
    int sbH;
    
    int scW;
    int scH;
    
    int sbRoundness;

	int moveAmty;
    int sbGap;
    ofColor sbColor;
    ofColor scColor;
    
	void isDraggingHandler(ofVec2f & _p) {
		moveContent(_p);
	}

public:
	void enableInteraction() {
#ifdef TOUCH_ENABLE
		ofAddListener(ofEvents().touchMoved, this, &ofxFilikaContentScroller::moveContentTouchHandler);
#else
		ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaContentScroller::moveMouseContent);
		ofAddListener(ofEvents().mousePressed, this, &ofxFilikaContentScroller::moveMouseContentPressed);
#endif	
		if (isScrollBarVisible)
			ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);

		ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);

		isScrollBarVisible = false;
	}

	void disableInteraction() {
		
#ifdef TOUCH_ENABLE
		ofRemoveListener(ofEvents().touchMoved, this, &ofxFilikaContentScroller::moveContentTouchHandler);
#else
		ofRemoveListener(ofEvents().mousePressed, this, &ofxFilikaContentScroller::moveMouseContentPressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxFilikaContentScroller::moveMouseContent);
#endif	
		if (isScrollBarVisible)
			ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);

		isScrollBarVisible = false;
		
	}
    void setup(ofRectangle _contentRect, ofBaseDraws * _content = nullptr, int _sbGap = 1) {
        this->set(_contentRect);
        
        
        sbGap   = _sbGap;
        sbColor = ofColor(150,150,150,60);
        scColor = ofColor(150,150,150,255);
        sbW     = 15;
        sbH     = 0;
        scW     = 15;
        scH     = 0;
		moveAmty = 0;

		if (_content != nullptr)
			setContent(_content);
		
		enableInteraction();
    }

#ifdef TOUCH_ENABLE
	void moveContentTouchHandler(ofTouchEventArgs  & e) {
		ofLog() << "TOUCH e.x: " << e.x << " e.y:" << e.y;
	}
#else
	void moveMouseContentPressed(ofMouseEventArgs & e) {
		if (e.x > this->x && e.x < this->x + this->getWidth() && e.y > this->y && e.y < this->y + this->getHeight()) {
			//saveX = e.x - scrollerBtn.getPos().x;
			ofLog() << "mn oluyor";
			saveY = e.y;
		}
	}

	void moveMouseContent(ofMouseEventArgs & e) {
		//
		if (e.x > this->x && e.x < this->x + this->getWidth() && e.y > this->y && e.y < this->y + this->getHeight()) {
			ofLog() << "TOUCH e.x: " << e.y << " e.y:" << e.y - saveY;
			
			//moveAmty = scrollBarRect.y + ofMap(e.y, scrollBarRect.y, scrollBarRect.getHeight(), scrollBarRect.y, scrollBarRect.y + content->getHeight() - scrollBarRect.height)*1;

			/*contentFbo.begin();
			ofClear(0, 0);
			content->draw(0, e.y - saveY);
			contentFbo.end();*/
			moveContent(ofVec2f(0, e.y ),1);
		}
			
	}
#endif	

	void moveContent(ofVec2f _p, int _dir = -1) {

		
		scrollerRect.y = _p.y;
		if (_p.y < scrollBarRect.y) {
			scrollerRect.y = scrollBarRect.y;
		}

		int maxYVal = scrollBarRect.y + scrollBarRect.height - scrollerBtn.getHeight();
		if (_p.y > maxYVal) {
			scrollerRect.y = maxYVal;
		}

		moveAmty = scrollBarRect.y + ofMap(scrollerRect.y, scrollBarRect.y, maxYVal, scrollBarRect.y, scrollBarRect.y + content->getHeight() - scrollBarRect.height)*_dir;

		contentFbo.begin();
		ofClear(0, 0);
		content->draw(0, moveAmty);
		contentFbo.end();
	}

    void draw() {
		if (content != nullptr)
		{
			contentFbo.getTexture().setAlphaMask(mask.getTexture());
			//ofDrawRectangle(x, y, width, height );
        
			contentFbo.draw(x,y);
        
			if(isScrollBarVisible) {
				ofPushStyle();
            
				// Scroll Bar
				ofSetColor(sbColor);
				ofDrawRectRounded(scrollBarRect, sbRoundness, sbRoundness, sbRoundness, sbRoundness);
            
				// Scroller
				//ofSetColor(scColor);
				//ofDrawRectRounded(scrollerRect, sbRoundness, sbRoundness, sbRoundness, sbRoundness);
				scrollerBtn.draw(scrollerRect.x,scrollerRect.y);
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
			scrollerBtn.setPivot("tl");
			scrollerBtn.setRoundness(sbRoundness);
			scrollerBtn.setDraggingVertical(true);
			scrollerBtn.setInteractionArea(ofVec2f(scrollerRect.width + 40, scrollerRect.height));

			if(isScrollBarVisible)
				ofRemoveListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);


			isScrollBarVisible = true;

			
			ofAddListener(scrollerBtn.BUTTON_DRAGGING, this, &ofxFilikaContentScroller::isDraggingHandler);
		}
		else {
			isScrollBarVisible = false;
		}
    }
    
	void setPositionScrobber(int _y) {
		//scrollerBtn.setPos();
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
    
    void setScrollerColor(ofColor _v) {
        scColor = _v;
    }
};

#endif /* ofxFilikaContentScroller_h */
