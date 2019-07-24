#pragma once
#include "ofMain.h"
#include "ofxHapPlayer.h"

/* DEPENDENCIES */
/* ofxHapPlayer */

class ofxFilikaHapPlayer
{
	private:
		/* PRIVATE VARIABLES */
		ofXml xml;
		string vidFolder;
		float vidSpeed;
		float vidVolume;
		bool vidIsLooping;
		bool loadSound;

		string vidDispModeW;
		string vidDispModeH;

		int vidW;
		int vidH;
		int vidX;
		int vidY;

		// Dir listing
		ofDirectory dir, dirSound;
		vector<ofxHapPlayer *> player;
		vector<ofSoundPlayer> sounds;
		int currentVid;

		bool autoPlay;
		
	public:
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

		void setPauseVideo() {
			player[currentVid]->setPaused(true);
			if(loadSound)
				sounds[currentVid].setPaused(true);
		}

		void setPosition(float _p) {
			player[currentVid]->setPosition(_p);
			if (loadSound)
				sounds[currentVid].setPosition(_p);
		}

		void setPlayVideo() {
			player[currentVid]->play();
			if (loadSound)
				sounds[currentVid].play();
		}

		void setup(string _settings, bool _autoPlay = false, bool _loadSound = false) {
			
			loadSound = _loadSound;
			autoPlay = _autoPlay;
			// Parse SETTINGS XML
			xml.load(_settings);

			vidFolder = xml.find("//root").getFirst().getChild("SOURCE_FOLDER").getValue();
			vidSpeed = ofToFloat(xml.find("//root").getFirst().getChild("VID_SPEED").getValue());
			vidVolume = ofToFloat(xml.find("//root").getFirst().getChild("VID_VOLUME").getValue());
			vidIsLooping = ofToBool(xml.find("//root").getFirst().getChild("VID_IS_LOOPING").getValue());

			vidX		 = ofToInt(xml.find("//root").getFirst().getChild("VID_X").getValue());
			vidY		 = ofToInt(xml.find("//root").getFirst().getChild("VID_Y").getValue());

			vidDispModeW = xml.find("//root").getFirst().getChild("VID_W").getValue();
			vidDispModeH = xml.find("//root").getFirst().getChild("VID_H").getValue();

			currentVid	 = ofToInt(xml.find("//root").getFirst().getChild("VID_ID_TO_START").getValue());

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
			parsePlaylistFolder();
		    
		}

		void parsePlaylistFolder() {

			dir.allowExt("avi");
			dir.allowExt("mov");


			if (dir.doesDirectoryExist(vidFolder)) {
				cout << "Playlist loaded succesfully" << endl;

				dir.listDir(vidFolder);
				dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

				player.resize(dir.size());

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
			}

			if (loadSound)
			{
				if (dirSound.doesDirectoryExist(vidFolder)) {
					cout << "sounds loaded succesfully" << endl;

					dirSound.listDir("sounds");
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
					cout << "Check sounds folder name..." << endl;
				}
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
			
		}

		void loadNext() {
			if (dir.size() > 0) {
				currentVid++;
				currentVid %= dir.size();
				player[currentVid]->play();
				if(loadSound)
					sounds[currentVid].play();
			}
		}

		void loadVideoById(int _id) {
			currentVid = _id;
			if (loadSound) {
				sounds[currentVid].stop();
				sounds[currentVid].play();
			}
			
			player[currentVid]->stop();
			player[currentVid]->play();
		}
		/*void setVideoPlayMode(int _id, bool _loopState)
		{
			player[currentVid]->play();
		}*/
};
