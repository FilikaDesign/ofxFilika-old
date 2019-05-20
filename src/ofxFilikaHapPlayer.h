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

		string vidDispModeW;
		string vidDispModeH;

		int vidW;
		int vidH;
		int vidX;
		int vidY;

		// Dir listing
		ofDirectory dir;
		vector<ofxHapPlayer *> player;
		int currentVid;

		bool autoPlay;
		
	public:
		~ofxFilikaHapPlayer() {
			for (int i = 0; i < (int)dir.size(); i++) {
				delete player[i];
			}
		}

		/* SETTERS & GETTERS */
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

		void setPlayVideo() {
			player[currentVid]->play();
		}

		void setup(string _settings, bool _autoPlay = false) {
			
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
		}

		void draw() {
			if (player[currentVid]->isLoaded())
			{
				if(player[currentVid]->isPlaying())
					player[currentVid]->draw(vidX,vidY,vidW,vidH);

				if (player[currentVid]->getIsMovieDone()) {
					if (player[currentVid]->getLoopState() == OF_LOOP_NORMAL)
						loadNext();
					else
						player[currentVid]->stop();
				}
			}
		}

		void loadNext() {
			if (dir.size() > 0) {
				currentVid++;
				currentVid %= dir.size();
				player[currentVid]->play();
			}
		}

		/*void setVideoPlayMode(int _id, bool _loopState)
		{
			player[currentVid]->play();
		}*/
};
