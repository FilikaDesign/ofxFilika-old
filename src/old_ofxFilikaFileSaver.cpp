/*#include "ofxFilikaFileSaver.h"


string ofxFilikaFileSaver::getFileName() {
	return _fileName;
}


ofxFilikaFileSaver::ofxFilikaFileSaver()
{

}


ofxFilikaFileSaver::~ofxFilikaFileSaver()
{
	ofLogNotice("---------------------------------------");
	ofLogNotice("ofxFilikaFileSaver deconstructor method");
	ofLogNotice("---------------------------------------");
}

void ofxFilikaFileSaver::setup(){
	imageW = globalSettings::camWidth;
	imageH = globalSettings::camHeight;
	this->setImageFileFormat("jpg");

	float ww = globalSettings::camWidth;
	float www = globalSettings::camWidthScreen;
	imgRatio = ww / www;

}

void ofxFilikaFileSaver::saveImageFile(string folderName, string _flName, ofPixels pixels){


	if (_flName == "") { //create timestamp name

		_fileName = folderName + "/" + ofGetTimestampString() + "." + fileFormat;

	}else {
		_fileName = folderName + "/" + _flName;
	}
	
	ofSaveImage(pixels, _fileName);

	try
	{
		std::string path = ofToDataPath(_fileName, true);
		ofx::IO::ByteBuffer byteBuffer;
		bool result = ofx::IO::ByteBufferUtils::loadFromFile(path, byteBuffer);
		if (result)	{
			ofLogNotice("ofApp::setup") << "File written";
		}else {
			ofLogError("ofApp::setup") << "File not written";
		}
	}
	catch (Poco::Exception& exception)
	{
		ofLogError("ofApp::setup") << "Exception: " << exception.displayText();
	}
}

void ofxFilikaFileSaver::setImageFileFormat(string _fileFormat){
	fileFormat = _fileFormat;
}





void ofxFilikaFileSaver::threadedFunction() {

	while (isThreadRunning())
	{
		// Attempt to lock the mutex.  If blocking is turned on,
		if (lock())
		{
			ofSaveImage(pixels, folderName + "/" + _fileName);
			//ofLog() << "is thread stopped";
			unlock();
			stopThread();
		}
	}
}

ofPixels ofxFilikaFileSaver::getPixels() {
	unique_lock<std::mutex> lock(mutex);

	return pixels;
}


void ofxFilikaFileSaver::combinePixels(string _folderName, ofPixels & _photoPx, ofPixels & _stickerPx, ofPixels & _drawingPx, ofPixels & _framePx)
{
	
	folderName = _folderName;
	ofImage drawing;

	printReady.clear();
	photo.clear();
	frame.clear();
	drawing.clear();
	stickers.clear();
	pixels.clear();

	drawing.allocate(globalSettings::camWidthScreen, globalSettings::camHeightScreen, ofImageType::OF_IMAGE_COLOR);
	stickers.allocate(globalSettings::camWidthScreen, globalSettings::camHeightScreen, ofImageType::OF_IMAGE_COLOR);

	thefbo.clear();
	thefbo.allocate(globalSettings::camWidth, globalSettings::camHeight, GL_RGB, 4);

	photo.setFromPixels(_photoPx);
	frame.setFromPixels(_framePx);
	drawing.setFromPixels(_drawingPx);
	stickers.setFromPixels(_stickerPx);

	thefbo.begin();
	ofSetColor(255, 255);


	photo.draw(globalSettings::photoOffset*imgRatio, globalSettings::photoOffset*imgRatio, globalSettings::camWidth - globalSettings::photoOffset*2*imgRatio, globalSettings::camHeight - globalSettings::photoOffset*2*imgRatio);
	frame.draw(0, 0, globalSettings::camWidth, globalSettings::camHeight);
	drawing.draw(0, 0, globalSettings::camWidth, globalSettings::camHeight);
	stickers.draw(0, 0, globalSettings::camWidth, globalSettings::camHeight);
	thefbo.end();


	thefbo.readToPixels(pixels);

	printReady.allocate(globalSettings::camWidth, globalSettings::camHeight, ofImageType::OF_IMAGE_COLOR);
	printReady.setFromPixels(pixels);

	_fileName = ofGetTimestampString() + ".jpg";
	startThread();
}
*/