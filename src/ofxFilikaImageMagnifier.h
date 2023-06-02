#pragma once

#ifndef OFXFILIKAIMAGEMAGNIFIER_H
#define OFXFILIKAIMAGEMAGNIFIER_H

#include "ofMain.h"

class ofxFilikaImageMagnifier {
private:
    ofImage img;
    int cw,ch,sx,sy,sw,sh;
    float sclFac;
    int frameThickness = 3;
    ofColor frameCol = ofColor::red;

public:
    ofxFilikaImageMagnifier() {

    }

    ~ofxFilikaImageMagnifier() {

    }

    void setup(ofImage & src, int frameW = 200, int frameH = 200, float _sclFac = 0.3) {
        img = src;

        cw = frameW;     // Magnifier Frame width
        ch = frameH;     // Magnifier Frame height
        sclFac = _sclFac; // Manifier scale amount <smaller the value larger the image>
        sw = cw * sclFac;
        sh = ch * sclFac;
    }

    void draw(int x, int y) {
        sx = x - cw*0.5;
        sy = y - ch*0.5;

        ofPushStyle();
        ofPushMatrix();
        ofSetLineWidth(frameThickness);
        ofTranslate(sx,sy);
        img.drawSubsection(0, 0, cw, ch, sx+sw, sy+sh, sw, sh);


        ofNoFill();
        ofSetColor(frameCol);
        ofDrawRectangle(0,0,cw,ch);
        ofPopMatrix();
        ofPopStyle();
    }

private:
};

#endif // OFXFILIKAIMAGEMAGNIFIER_H

/*
EXAMPLE
---------------------------------------------------------------

ofApp.h

---------------------------------------------------------------

#include "ofMain.h"
#include "ofxFilikaImageMagnifier.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofImage img;
    bool isPressed ;
    ofxFilikaImageMagnifier mag;
};


---------------------------------------------------------------

ofApp.cpp

---------------------------------------------------------------

void ofApp::setup(){
    ofSetVerticalSync(true);
    string str =  "/Users/alptugan/oF/assets/test.jpg";

    img.load(str);

    mag.setup(img);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw the original image
    ofSetColor(ofColor::white);
    img.draw(0, 0);

    if(isPressed) {
        int x = ofGetWidth()*0.5;
        int y = img.getHeight()*0.5;
        if(mouseX > 0 && mouseX < ofGetWidth() && mouseY > img.getHeight()) {
            x = ofMap(mouseX, 0, ofGetWidth(), 0, ofGetWidth());
            y = ofMap(mouseY, img.getHeight(), ofGetHeight(), 0, img.getHeight());
        }

        mag.draw(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    isPressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    isPressed = false;
}


*/
