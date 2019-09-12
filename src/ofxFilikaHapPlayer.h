#pragma once
#include "ofMain.h"
#include "ofxHapPlayer.h"
#include "ofxFilikaUtils.h"
#include "ofxFilikaDraggableButton.h"

/* DEPENDENCIES */
/* ofxHapPlayer */
/* ofxFilika */

/* USAGE */
/* You should include;
 1- A video foler
 2- An xml settings file as follows
 */
/****************************************************************************
XML SETTINGS STRUCTURE
*****************************************************************************
<?xml version="1.0" encoding="utf-8"?>
 <root>
     <SOURCE_FOLDER>/Users/alptugan/oF/assets/hap</SOURCE_FOLDER>
     <SOUND_FOLDER></SOUND_FOLDER>
     <COVER_FOLDER></COVER_FOLDER>
     <VID_SPEED>1</VID_SPEED>
     <VID_VOLUME>1</VID_VOLUME>
     <VID_IS_LOOPING>1</VID_IS_LOOPING>
     <VID_X>0</VID_X>
     <VID_Y>0</VID_Y>
     <VID_W>full</VID_W>
     <VID_H>full</VID_H>
     <VID_ID_TO_START>0</VID_ID_TO_START>
     <BTN_PLAY>btnplayw.png</BTN_PLAY>
     <BTN_PAUSE>btnpausew.png</BTN_PAUSE>
 </root>
 *****************************************************************************
 INIT IN A PROJECT
 In header file;
 ofxFilikaHapPlayer player;
 
 In .cpp file;
 player.setup("path_to_settings_file.xml", bool autoplay = false, bool loadsound = false, bool navigation = false, bool enableCoverImages = false);
 
*/

class ofxFilikaHapPlayer
{
	private:

		/* PRIVATE VARIABLES */
        ofTrueTypeFont ff;
        vector<ofImage> coverImages;
    
        int stW;
    
		ofXml xml;
        ofxFilikaUtils utils;
		string vidFolder;
        string soundFolder;
        string coverFolder;
		float vidSpeed;
		float vidVolume;
		bool vidIsLooping;
		bool loadSound;
        bool showCover;

		string vidDispModeW;
		string vidDispModeH;

		int vidW;
		int vidH;
		int vidX;
		int vidY;
        int btnPW;
    
        bool isNavEnabled;

		// Dir listing
		ofDirectory dir, dirSound, dirImages;
		vector<ofxHapPlayer *> player;
		vector<ofSoundPlayer> sounds;
		int currentVid;

		bool autoPlay;

        float lastMovement;
        ofColor vidBarStrokeColor, vidBarFillColor;
        int navBarMargin, navBarH;
        bool wasPaused;
    
        bool inNavBar, isNavBarAutoHide, drawNavBar;
        float navBarAutoHideTime;
    
        string srcPlay,srcPause;
    
        ofxFilikaDraggableButton btnPlayPause;
        ofxFilikaDraggableButton btnPlayBig;
    
        bool isCoverImagesEnabled;
	public:

		void enableInteraction() {
			// If navigation enabled
			if (isNavEnabled) {
				// Init Play/Pause button
				if (srcPlay != "" || srcPause != "")
				{
					ofAddListener(btnPlayPause.BUTTON_TOUCH_UP, this, &ofxFilikaHapPlayer::onPlayPauseDown);
					ofAddListener(btnPlayBig.BUTTON_TOUCH_UP, this, &ofxFilikaHapPlayer::onPlayBigDown);
				}
//#ifdef TOUCH_ENABLE
				ofAddListener(ofEvents().touchDown, this, &ofxFilikaHapPlayer::touchDown);
				ofAddListener(ofEvents().touchUp, this, &ofxFilikaHapPlayer::touchUp);
				ofAddListener(ofEvents().touchMoved, this, &ofxFilikaHapPlayer::touchMoved);
//#else
				ofAddListener(ofEvents().mousePressed, this, &ofxFilikaHapPlayer::mousePressed);
				ofAddListener(ofEvents().mouseReleased, this, &ofxFilikaHapPlayer::mouseReleased);
				ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaHapPlayer::mouseDragged);
				ofAddListener(ofEvents().mouseMoved, this, &ofxFilikaHapPlayer::mouseMoved);
//#endif
			}
		}

		void disableInteraction() {
			// If navigation enabled
			if (isNavEnabled) {
				// Init Play/Pause button
				if (srcPlay != "" || srcPause != "")
				{
					ofRemoveListener(btnPlayPause.BUTTON_TOUCH_UP, this, &ofxFilikaHapPlayer::onPlayPauseDown);
					ofRemoveListener(btnPlayBig.BUTTON_TOUCH_UP, this, &ofxFilikaHapPlayer::onPlayBigDown);
				}
//#ifdef TOUCH_ENABLE
				ofRemoveListener(ofEvents().touchDown, this, &ofxFilikaHapPlayer::touchDown);
				ofRemoveListener(ofEvents().touchUp, this, &ofxFilikaHapPlayer::touchUp);
				ofRemoveListener(ofEvents().touchMoved, this, &ofxFilikaHapPlayer::touchMoved);
//#else
				ofRemoveListener(ofEvents().mousePressed, this, &ofxFilikaHapPlayer::mousePressed);
				ofRemoveListener(ofEvents().mouseReleased, this, &ofxFilikaHapPlayer::mouseReleased);
				ofRemoveListener(ofEvents().mouseDragged, this, &ofxFilikaHapPlayer::mouseDragged);
				ofRemoveListener(ofEvents().mouseMoved, this, &ofxFilikaHapPlayer::mouseMoved);
//#endif
			}
		}
    
		~ofxFilikaHapPlayer() {
			for (int i = 0; i < (int)dir.size(); i++) {
				delete player[i];
			}
		}

		/* SETTERS & GETTERS */
		bool getIsVideoPlaying() {
			return player[currentVid]->isPlaying();
		}

		float getVideoPosition() {
			return player[currentVid]->getPosition();
		}

		void setSpeed(float _s,int _i) {
			player[_i]->setSpeed(_s);
		}

		void setVolume(float _v, int _i) {
			player[_i]->setVolume(_v);
		}

		void setLoop(bool _b,int _i) {
			if (_b)
				player[_i]->setLoopState(OF_LOOP_NORMAL);
			else
				player[_i]->setLoopState(OF_LOOP_NONE);
		}
    
        void setNavBarAutoHide(bool _autoHide, float _autoHideThres = 3000) {
            isNavBarAutoHide = _autoHide;
            navBarAutoHideTime = _autoHideThres;
            
            if(isNavEnabled) {
                if(isNavBarAutoHide)
                    drawNavBar = false;
                else
                    drawNavBar = true;
            }
        }

		void setVidBarStrokeColor(ofColor _col) {
			vidBarStrokeColor = _col;
		}

		void setVidBarFillColor(ofColor _col) {
			vidBarFillColor = _col;
		}
    
        /*void setPlayPauseBtnImages(string _playBtnSrc, string _pauseBtnSrc) {
            
        }*/
    
        /* Pause Video */
		void setPauseVideo() {
			player[currentVid]->setPaused(true);
			if(loadSound)
				sounds[currentVid].setPaused(true);
		}

		/* Stop Player */
		void stop() {
			player[currentVid]->stop();
			if (loadSound)
				sounds[currentVid].stop();
		}
    
        /* Goto position pct % */
		void setPosition(float _pct) {
			player[currentVid]->setPosition(_pct);
			if (loadSound)
				sounds[currentVid].setPosition(_pct);
		}
    
        /* Play Video */
		void setPlayVideo() {
			player[currentVid]->play();
			if (loadSound)
				sounds[currentVid].play();
		}
    
        /* Load Next Video in the queue */
        void loadNext() {
            if (dir.size() > 0) {
                currentVid++;
                currentVid %= dir.size();
                player[currentVid]->play();
                if(loadSound)
                    sounds[currentVid].play();
            }
        }
    
        /* Load video by id from the playlist */
        void loadVideoById(int _id) {
            currentVid = _id;
            if (loadSound) {
                sounds[currentVid].stop();
                sounds[currentVid].play();
            }
            
            player[currentVid]->stop();
            player[currentVid]->play();
        }

		void loadVideoByName(string _src) {
			player[currentVid]->load(_src);
		}

		void setSize(int _w, int _h) {
			vidW = _w;
			vidH = _h;
		}

		void setPlayerPosition(int _xpos, int _ypos) {
			vidX = _xpos;
			vidY = _ypos;
		}

		void setup(string _settings, bool _autoPlay = false, bool _loadSound = false, bool _nav = false, bool _coverImages = false) {
			
			loadSound            = _loadSound;
			autoPlay             = _autoPlay;
            isNavEnabled         = _nav;
            isCoverImagesEnabled = _coverImages;
            
            // Constants
            vidBarFillColor      = ofColor(0,0,0,100);
            vidBarStrokeColor    = ofColor(255,255,255,255);
            stW                  = 12;
            navBarMargin         = 30;
            navBarH              = 30 - stW;
            navBarAutoHideTime   = 3000;
            isNavBarAutoHide     = false;
            showCover            = false;
            
            
            // If autoplay = false then show cover image for the video
            if(!autoPlay && isCoverImagesEnabled)
                showCover = true;
            
            if(isNavEnabled) {
                // LOAD DISPLAY FONT FOR REMAINING AND PASSED TIME
                ff.load(OF_TTF_SANS, 10);
                
                drawNavBar = true;
                
                if(isNavBarAutoHide)
                    drawNavBar = false;
            }
            
            
            
            
			// Parse SETTINGS XML
			xml.load(_settings);

			vidFolder    = xml.find("//root").getFirst().getChild("SOURCE_FOLDER").getValue();
			vidSpeed     = ofToFloat(xml.find("//root").getFirst().getChild("VID_SPEED").getValue());
			vidVolume    = ofToFloat(xml.find("//root").getFirst().getChild("VID_VOLUME").getValue());
			vidIsLooping = ofToBool(xml.find("//root").getFirst().getChild("VID_IS_LOOPING").getValue());

			vidX		 = ofToInt(xml.find("//root").getFirst().getChild("VID_X").getValue());
			vidY		 = ofToInt(xml.find("//root").getFirst().getChild("VID_Y").getValue());

			vidDispModeW = xml.find("//root").getFirst().getChild("VID_W").getValue();
			vidDispModeH = xml.find("//root").getFirst().getChild("VID_H").getValue();

			currentVid	 = ofToInt(xml.find("//root").getFirst().getChild("VID_ID_TO_START").getValue());
            srcPlay      = xml.find("//root").getFirst().getChild("BTN_PLAY").getValue();
            srcPause     = xml.find("//root").getFirst().getChild("BTN_PAUSE").getValue();
            
            if(loadSound) {
                soundFolder = xml.find("//root").getFirst().getChild("SOUND_FOLDER").getValue();
                if(soundFolder == "")
                    soundFolder = "sounds";
            }
            
            if( isCoverImagesEnabled ) {
                coverFolder = xml.find("//root").getFirst().getChild("COVER_FOLDER").getValue();
                if(coverFolder == "")
                    coverFolder = "covers";
            }
            
            
			if (vidDispModeW == "full") {
				vidW = ofGetWidth();
			}
			else {
				vidW = ofToInt(xml.find("//root").getFirst().getChild("VID_W").getValue());
			}

			if (vidDispModeH == "full") {
				vidH = ofGetHeight();
			}
			else {
				vidH = ofToInt(xml.find("//root").getFirst().getChild("VID_H").getValue());
			}

			// Load a movie file
			if (vidFolder != "")
			{
				parsePlaylistFolder();
			}
			else {
				ofLog() << "switch to single player mode";
				player.resize(1);
				sounds.resize(1);
				player[0] = new ofxHapPlayer();
				currentVid = 0;
			}
            // If navigation enabled
            if(isNavEnabled) {
                // Init Play/Pause button
                if(srcPlay != "" || srcPause != "")
                {
                    ofAddListener(btnPlayPause.BUTTON_TOUCH_UP, this, &ofxFilikaHapPlayer::onPlayPauseDown);
                    ofAddListener(btnPlayBig.BUTTON_TOUCH_UP, this, &ofxFilikaHapPlayer::onPlayBigDown);
                    btnPlayPause.setup(srcPlay, 1);
                    btnPlayPause.setPassiveImage(srcPause);
                    
                    btnPlayBig.setup(srcPlay, 2);
                    btnPlayBig.setButtonSize(150, 150);
                    
                    btnPlayPause.setButtonSize(navBarH + stW * 2, navBarH + stW * 2);
                }else{
                    cout << "There is no play/pause button file data in XML file" << endl;
                    return;
                }
                
//#ifdef TOUCH_ENABLE
                ofAddListener(ofEvents().touchDown, this,  &ofxFilikaHapPlayer::touchDown);
                ofAddListener(ofEvents().touchUp, this,  &ofxFilikaHapPlayer::touchUp);
                ofAddListener(ofEvents().touchMoved, this, &ofxFilikaHapPlayer::touchMoved);
//#else
                ofAddListener(ofEvents().mousePressed, this,  &ofxFilikaHapPlayer::mousePressed);
                ofAddListener(ofEvents().mouseReleased, this,  &ofxFilikaHapPlayer::mouseReleased);
                ofAddListener(ofEvents().mouseDragged, this, &ofxFilikaHapPlayer::mouseDragged);
                ofAddListener(ofEvents().mouseMoved, this, &ofxFilikaHapPlayer::mouseMoved);
//#endif
            }
		}
    
		void draw() {
			if(loadSound)
				ofSoundUpdate();


			if (loadSound) {
				if (player[currentVid]->isLoaded() && sounds[currentVid].isLoaded())
				{
					if (player[currentVid]->isPlaying())
						player[currentVid]->draw(vidX, vidY, vidW, vidH);

					///if (sounds[currentVid].isPlaying())

					/*if (player[currentVid]->getIsMovieDone()) {
						if (player[currentVid]->getLoopState() == OF_LOOP_NORMAL)
							loadNext();
						else
							player[currentVid]->stop();
					}*/
				}
			}
			else {
				if (player[currentVid]->isLoaded())
				{
					if (player[currentVid]->isPlaying())
						player[currentVid]->draw(vidX, vidY, vidW, vidH);
				}
			}
            
            // If navigation enabled then draw it on screen in front of the video
            if(isNavEnabled) {
                if(isNavBarAutoHide) {
                    if ((ofGetSystemTimeMillis() - lastMovement < navBarAutoHideTime))
                    {
                        drawNavBar = true;
                    }
                    else
                    {
                        drawNavBar = false;
                    }
                }else{
                    drawNavBar = true;
					
                }
                
                if(showCover && isCoverImagesEnabled) {
                    coverImages[currentVid].draw(vidX, vidY, vidW, vidH);
                    ofPushStyle();
                    ofSetColor(0,0,0,130);
                    ofDrawRectangle(vidX, vidY, vidW, vidH);
                    
                    btnPlayBig.draw(vidX + vidW * 0.5 - btnPlayBig.getWidth() * 0.5, vidY + vidH * 0.5 - btnPlayBig.getHeight() * 0.5);
                    ofPopStyle();
                }
                
                if(player[currentVid]->isPaused() || player[currentVid]->getIsMovieDone()) {
                    //showCover = true;
                }
                
                if(drawNavBar && showCover == false)
                {
                    // Scrubbing
                    ofPushStyle();
                    ofRectangle bar = getBarRectangle();
                    ofFill();
                    ofSetColor(vidBarStrokeColor);
                    ofDrawRectRounded(ofRectangle(bar.x - stW*0.5, bar.y - stW*0.5, bar.getWidth() + stW, bar.getHeight() + stW), 30);
                    ofNoFill();
                   
                    ofSetLineWidth(stW);
                    ofSetColor(vidBarStrokeColor);
                    //ofDrawRectangle(bar);
                    ofDrawRectRounded(bar, 30);
                    
                    ofFill();
                    ofRectangle barFill = getBarRectangle();
                    ofSetColor(vidBarFillColor);
                    barFill.width *= player[currentVid]->getPosition();
                    //ofDrawRectangle(bar);
                    ofDrawRectRounded(barFill, 30);
                    ofPopStyle();
                    
                    // Place play/pause Button
                    if(player[currentVid]->isPaused() || player[currentVid]->getIsMovieDone()) {
                        btnPlayPause.setPassive(false);
                        //showCover = true;
                    }else{
                        //showCover = false;
                        btnPlayPause.setPassive(true);
                    }
                    btnPlayPause.draw(vidX + navBarMargin, barFill.y + barFill.getHeight() * 0.5 - btnPlayPause.getHeight() * 0.5);
                    
                    btnPW = btnPlayPause.getPos().x + btnPlayPause.getWidth() * 0.5;
                    
                    // Passed and remaining time
                    ofPushStyle();
                    float passedTime = ofMap(player[currentVid]->getPosition(), 0, 1, 0, player[currentVid]->getDuration());
                    string remainingTime = utils.calculateTime(player[currentVid]->getDuration() - passedTime);
                    string totalTime = utils.calculateTime(player[currentVid]->getDuration());
                    //ofBitmapFont f;
                    
                    ofRectangle rectPassedTime = ff.getStringBoundingBox(remainingTime + " / " + totalTime,0,0);
                   
                    int passedTimeX = bar.x + bar.getWidth() - rectPassedTime.getWidth() - 10;
                    int passedTimeY = bar.y + (bar.getHeight() * 0.5 + 5);
                    
                    // ofSetColor(255);
                    // ofDrawBitmapString(ofToString("00:00"), passedTimeX + 1, passedTimeY + 1);
                    ofSetColor(0);
                    //ofDrawBitmapString(remainingTime + "/" + totalTime, passedTimeX, passedTimeY);
                    ff.drawString(remainingTime + " / " + totalTime, passedTimeX, passedTimeY);
                    ofPopStyle();
                }
            }
		}

		
		/*void setVideoPlayMode(int _id, bool _loopState)
		{
			player[currentVid]->play();
		}*/
private:
    void onPlayPauseDown(int & _id) {
        
		if (loadSound) {
			sounds[currentVid].setPaused(!player[currentVid]->isPaused());
		}

        player[currentVid]->setPaused(!player[currentVid]->isPaused());

        
        if(player[currentVid]->isPaused() && isCoverImagesEnabled) {
            showCover = true;
        }
    }
    
    void onPlayBigDown(int & _id) {
        
		if (isCoverImagesEnabled) {
			player[currentVid]->play();
			showCover = false;
		}
    }

    ofRectangle getBarRectangle() const
    {
        return ofRectangle(navBarMargin + btnPW, vidY + vidH - navBarMargin - navBarH, vidX + vidW - (2 * navBarMargin) - btnPW, navBarH);
    }
    
//#ifdef TOUCH_ENABLE
    void touchDown(ofTouchEventArgs & args) {
        pointerDown(glm::vec2(args.x, args.y));
    }
    
    void touchUp(ofTouchEventArgs & args) {
        pointerUp(glm::vec2(args.x, args.y));
    }
    
    void touchMoved(ofTouchEventArgs & args) {
        pointerDragged(glm::vec2(args.x, args.y));
    }
//#else
    void mousePressed(ofMouseEventArgs & args) {
        pointerDown(glm::vec2(args.x, args.y));
    }
    
    void mouseReleased(ofMouseEventArgs & args) {
        pointerUp(glm::vec2(args.x, args.y));
    }
    
    void mouseDragged(ofMouseEventArgs & args) {
        pointerDragged(glm::vec2(args.x, args.y));
    }
    
    void mouseMoved(ofMouseEventArgs & args) {
        if (isNavBarAutoHide && ofGetWindowRect().inside(args.x, args.y))
        {
            lastMovement = ofGetSystemTimeMillis();
        }
    }
//#endif
    
    void pointerDown(glm::vec2 args) {
        ofRectangle bar = getBarRectangle();
        if (bar.inside(args.x, args.y))
        {
            showCover = false;
            inNavBar = true;
            wasPaused = player[currentVid]->isPaused() || player[currentVid]->getIsMovieDone();
            player[currentVid]->setPaused(true);

			if (loadSound) {
				sounds[currentVid].setPaused(true);
			}

            pointerDragged(glm::vec2(args.x, args.y));
        }
        
        if(isNavBarAutoHide)
            lastMovement = ofGetSystemTimeMillis();
    }
    
    void pointerUp(glm::vec2 args) {
        if (inNavBar)
        {
            inNavBar = false;
            player[currentVid]->setPaused(wasPaused);

			if (loadSound) {
				sounds[currentVid].setPaused(wasPaused);
			}
        }
    }
    
    void pointerDragged(glm::vec2 args) {
        if (inNavBar)
        {
            showCover = false;
            float position = static_cast<float>(args.x - navBarMargin - btnPW) / getBarRectangle().width;
            position = std::max(0.0f, std::min(position, 1.0f));
            player[currentVid]->setPosition(position);

			if (loadSound) {
				sounds[currentVid].setPosition(position);
			}
            
            if(isNavBarAutoHide)
                lastMovement = ofGetSystemTimeMillis();
        }
    }
    
    void parsePlaylistFolder() {
        
        dir.allowExt("avi");
        dir.allowExt("mov");
        
        
        if (dir.doesDirectoryExist(vidFolder)) {
            cout << "Playlist folder exist" << endl;
            
            dir.listDir(vidFolder);
            dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
            
            player.resize(dir.size());
            
            if(dir.size() < 1) {
                cout << "Folder is empty" << endl;
            }
            
            
            // you can now iterate through the files and load them into the ofImage vector
            for (int i = 0; i < (int)dir.size(); i++) {
                player[i] = new ofxHapPlayer();
                player[i]->load(dir.getPath(i));
                setLoop(vidIsLooping, i);
                setVolume(vidVolume, i);
                setSpeed(vidSpeed, i);
            }
            //currentVid = 0;
            currentVid = currentVid % dir.size();
            
            if(autoPlay)
                player[currentVid]->play();
        }
        else {
            cout << "Check playlist folder name..." << endl;
            return;
        }
        
        if (loadSound)
        {
            dirSound.allowExt("mp3");
            dirSound.allowExt("wav");
            dirSound.allowExt("ogg");
            dirSound.allowExt("aif");
            dirSound.allowExt("aiff");
            dirSound.allowExt("m4a");
            
            if (dirSound.doesDirectoryExist(soundFolder)) {
                cout << soundFolder << " folder exist" << endl;
                
                dirSound.listDir(soundFolder);
                dirSound.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
                
                sounds.resize(dirSound.size());
                
                // you can now iterate through the files and load them into the ofImage vector
                for (int i = 0; i < (int)dirSound.size(); i++) {
                    sounds[i].load(dirSound.getPath(i));
                    sounds[i].setLoop(vidIsLooping);
                    sounds[i].setVolume(vidVolume);
                    sounds[i].setSpeed(vidSpeed);
                }
                
                if (autoPlay)
                    sounds[currentVid].play();
            }
            else {
                cout << "Check " << soundFolder << " folder name..." << endl;
            }
        }
        
        if(isCoverImagesEnabled) {
            if (dirImages.doesDirectoryExist(coverFolder)) {
                dirImages.allowExt("png");
                dirImages.allowExt("jpg");
                
                cout << coverFolder << " folder exist" << endl;
                
                dirImages.listDir(coverFolder);
                dirImages.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
                
                coverImages.resize(dirImages.size());
                
                // you can now iterate through the files and load them into the ofImage vector
                for (int i = 0; i < (int)dirImages.size(); i++) {
                    coverImages[i].load(dirImages.getPath(i));
                }
                
            }
            else {
                cout << "Check your covers folder name..." << endl;
            }
        }
    }
};
