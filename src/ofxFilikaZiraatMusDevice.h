#pragma once
#include "ofMain.h"

class ofxFilikaZiraatMusDevice {
private:
	ofXml xml;
	vector<string> responses;
public:
	void setup(string _src) {
		if (xml.load(_src))
			cout << "DEVICE OSC COMMANDS LOAD SUCCESS" << endl;
		else
			cout << "DEVICE OSC COMMANDS NOT LOADED" << endl;

		// SET RESPONSES VECTOR
		/*int bNum = xml.find("//root/DEVICE/RESPONSE").size();

		responses.resize(bNum);
		for (int i = 0; i < bNum; i++)
		{
			string bName = xml.find("//root/DEVICE")[i].getValue();
			responses[i] = bName;
			//cout << responses[i] << endl;
		}*/
	}

	int getTagValInt(string _tag) {
		return xml.find("//root").getFirst().getChild(_tag).getIntValue();
	}

	string getTagValStr(string _tag) {
		return xml.find("//root").getFirst().getChild(_tag).getValue();
	}

	string getTagValRecursive(string _first, string _second, int _id) {
		string bName = xml.find("//root/" + _first + "/" + _second)[_id].getValue();

		return bName;
	}

	int getDeviceId(string _name) {
		//cout << xml.find("//root/DEVICE").size() << endl;
		int _id;
		for (int i = 0; i < xml.find("//root/DEVICE").size(); i++)
		{
			string bName = xml.find("//root/DEVICE/DEV_NAME")[i].getValue();
			if (bName == _name) {
				_id = i;
				break;
			}
		}

		return _id;
	}

	string name;
	int oscPort;
	string cmdShutDown;
	string cmdRestart;
	string cmdStatusDisplay;
	string cmdStatusDisplayOn;
	string cmdStatusDisplayOff;
	string cmdCheckApp; 
};