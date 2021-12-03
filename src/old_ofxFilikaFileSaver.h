#pragma once
#include "ofMain.h"
#include "globalSettings.h"
#include "ofxIO.h"



class ofxFilikaFileSaver: public ofThread
{
public:
	ofxFilikaFileSaver();
	~ofxFilikaFileSaver();

	// Getters && Setters
	string getFileName();
	ofPixels pixels;

	void setup();
	void saveImageFile(string folderName, string fileName, ofPixels pixels);
	void setImageFileFormat(string _fileFormat);

	void threadedFunction();

	ofPixels getPixels();

	void combinePixels(string folderName, ofPixels & photoPx,  ofPixels & stickerPx, ofPixels & drawingPx, ofPixels &  framePx);


	string fileFormat;

	ofFbo thefbo;
	ofImage frame;
	ofImage photo;
	ofImage stickers;
	ofImage printReady;

private:
	string _fileName;
	string folderName;
	int imageW, imageH;
	float imgRatio;
};

