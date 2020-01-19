#pragma once
#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

#include "ofMain.h"
#include "ofxFilikaConstants.h"

#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600

class ofxFilikaUtils {
private:
	
	string xmlFile;
    ofBitmapFont bf;
    float nextEventSeconds = 0;
public:
	ofXml xml;
    #ifdef WIN32
	bool pingIpAdd(const char argv[], bool _verbose = false) {

		bool result;

		HANDLE hIcmpFile;
		unsigned long ipaddr = INADDR_NONE;
		DWORD dwRetVal = 0;
		char SendData[32] = "Data Buffer";
		LPVOID ReplyBuffer = NULL;
		DWORD ReplySize = 0;

		
		ipaddr = inet_addr(argv);
		if (ipaddr == INADDR_NONE) {
			if(_verbose)
				printf("usage: %s IP address\n", argv);
		}

		hIcmpFile = IcmpCreateFile();
		if (hIcmpFile == INVALID_HANDLE_VALUE) {
			if (_verbose)
			{
				printf("\tUnable to open handle.\n");
				printf("IcmpCreatefile returned error: %ld\n", GetLastError());
			}
			result = false;
		}

		ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
		ReplyBuffer = (VOID*)malloc(ReplySize);
		if (ReplyBuffer == NULL) {
			if (_verbose)
			{
				printf("\tUnable to allocate memory\n");
			}
			result = false;

		}


		dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
			NULL, ReplyBuffer, ReplySize, 1000);
		if (dwRetVal != 0) {
			PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
			struct in_addr ReplyAddr;
			ReplyAddr.S_un.S_addr = pEchoReply->Address;
			if (_verbose)
			{
				printf("\tSent icmp message to %s\n", argv);
			}
				if (dwRetVal > 1) {
					if (_verbose)
					{
						printf("\tReceived %ld icmp message responses\n", dwRetVal);
						printf("\tInformation from the first response:\n");
					}
			}
			else {
					if (_verbose)
					{
						printf("\tReceived %ld icmp message response\n", dwRetVal);
						printf("\tInformation from this response:\n");
					}
			}
				if (_verbose)
				{
					printf("\t  Received from %s\n", inet_ntoa(ReplyAddr));
					printf("\t  Status = %ld\n",pEchoReply->Status);
					printf("\t  Roundtrip time = %ld milliseconds\n",pEchoReply->RoundTripTime);
				}
			result = true;

		}
		else {
			if (_verbose)
			{
				printf("\tCall to IcmpSendEcho failed.\n");
				printf("\tIcmpSendEcho returned error: %ld\n", GetLastError());
			}
			result = false;

		}

		return result;
	}
    #endif
    
    string calculateTime(float _time) {
        
        int seconds;
        int minutes;
        int mins_left;
        int secs_left;
        
        seconds = (/*gameTimeSeconds - */int(_time));
        minutes = (/*gameTimeSeconds - */int(_time)) / SECS_PER_MIN;
        mins_left = minutes % SECS_PER_MIN;
        secs_left = seconds % SECS_PER_MIN;
        
        string mins;
        string secs;
        
        if(mins_left < 10) {
            mins = "0"+ofToString(mins_left);
        }else {
            mins = ofToString(mins_left);
        }
        
        if(secs_left < 10) {
            secs = "0"+ofToString(secs_left);
        }else {
            secs = ofToString(secs_left);
        }
        
        
        //cout << ofGetElapsedTimeMillis() / 1000 << endl;
        //cout << "remaining time : " << mins_left << " : " <<  secs_left << endl;
        //cout << "remaining time : " << mins << " : " <<  secs << endl;
        
        if(mins_left < 0 || secs_left < 0)
            return "00:00";
        else
            return (mins + ":" + secs);
    }

	/* GL RELATED - GRAPHICS */
	int getMaxTextureSize() {
		GLint maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);

		//ofLog() << "Max texture size : " << maxTexSize;

		return maxTexSize;
	}


	/* STRING OPERATIONS */
	/* Save ofxFilikaUtils.h file as unicode-(without signature) */
	string toUpperUTF8(string _txt) {
		string convert_i_to_cap = findAndReplaceAll(_txt, "i", "İ");
		string convertToCapitalsPre = findAndReplaceAll(convert_i_to_cap, "ı", "i");
		
		return ofToUpper(convertToCapitalsPre);
	}

	string findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
	{
		// Get the first occurrence
		size_t pos = data.find(toSearch);

		// Repeat till end is reached
		while (pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, toSearch.size(), replaceStr);
			// Get the next occurrence from the current position
			pos = data.find(toSearch, pos + replaceStr.size());
		}

		return data;
	}

	/* RETURN FILE EXTENSION */
	string getFileName(string _str, bool _trimExt = false) {
		string name = "";
		int slashPos = _str.rfind('/');

		if (slashPos != string::npos) {
			name = _str.substr(slashPos + 1);

			if (_trimExt)
				name = getFilePathTrimExtension(name);
		}

		return name;
	}

	string getFileFolderPath(string _str) {
		string name = "";
		int slashPos = _str.rfind('/');

		if (slashPos != string::npos) {
			name = _str.substr(0, slashPos);
		}

		return name;
	}

	string getFileExtension(string _str) {
		string ext = "";
		int extPos = _str.rfind('.');

		if (extPos != string::npos) {
			ext = _str.substr(extPos + 1);
		}

		return ext;
	}

	string getFilePathTrimExtension(string _str) {
		string ext = "";
		int extPos = _str.rfind('.');

		if (extPos != string::npos) {
			ext = _str.substr(0, extPos);
		}

		return ext;
	}

	int getNumWords(std::string str)
	{
		int word_count(0);
		std::stringstream ss(str);
		std::string word;
		while (ss >> word) ++word_count;
		return word_count;
	};
    
    /* CONVERT UNICODE(TR) TO LATIN(EN) CHARS */
    string convertToNonUnicodeText(string _str, string _spaceChar = "_") {
        string tr []= {"ı","İ","ş","Ş","ğ","Ğ","ç","Ç","ö","Ö","ü","Ü"," "};
        string en []= {"i","i","s","s","g","g","c","c","o","o","u","u",_spaceChar };
        
        int ko = sizeof(tr)/sizeof(string);
        
        for (int i = 0; i < ko; i++) {
            ofStringReplace(_str, tr[i], en[i]);
        }
        
        return _str;
    }
    
    /* CONVERT String To Char */
    /* Usage:
     char * foo = utils.convertStringToChar("Test String");
    */
    char * convertStringToChar(string _str) {
        char *foo = (char*)(&_str);
        return foo;
    }

	/* CONVERT String to const char argv[] 
	const char* convertStringToConstChar(string _str) {
		const char * foo;
		foo = _str.c_str();

		return foo;
	}*/
    
	/* XML Operations */
	void loadXml(string _src) {
		xmlFile = _src;
		xml.load(_src);
	}

	// should be called xml.find("//root/RESPONSES/RESPONSE").size();
	int getNumTags(string _tagPath) {
		return xml.find(_tagPath).size();
	}

	vector<string> getTagStringListRecursive(string _tagPath, int _id, string _tag1) {
		vector<string> list;
		int size = xml.find(_tagPath)[_id].find(_tag1).size();
		list.resize(size);

		for (int i = 0; i < list.size(); i++)
		{
			list[i] = xml.find(_tagPath)[_id].find(_tag1)[i].getValue();
		}

		return list;
	}

	vector<string> getTagStringList(string _tagPath) {
		vector<string> list;
		list.resize(xml.find(_tagPath).size());

		for (int i = 0; i < list.size(); i++)
		{
			list[i] = xml.find(_tagPath)[i].getValue();
		}

		return list;
	}

	vector<int> getTagIntList(string _tagPath) {
		vector<int> list;
		list.resize(xml.find(_tagPath).size());

		for (int i = 0; i < list.size(); i++)
		{
			list[i] = xml.find(_tagPath)[i].getIntValue();
		}

		return list;
	}

	/* First Level */
	string getPathValStr(string _path_xml_tag) {
		return xml.find(_path_xml_tag)[0].getValue();
	}

	string getTagValStr(string _tag) {
		return xml.find("//root").getFirst().getChild(_tag).getValue();
	}

	/* Second Level */
	string getTagValStr(string _tag, string _second_tag) {
		return xml.find("//root").getFirst().getChild(_tag).getChild(_second_tag).getValue();
	}

	void setTagIntValue(string _path, int _index, int _val, bool _autoSave = true) {
		xml.find(_path)[_index].set(_val);
		if(_autoSave) writeXml(xmlFile);
	}

	void setTagStringValue(string _path, int _index, string _val, bool _autoSave = true) {
		xml.find(_path)[_index].set(_val);
		if (_autoSave) writeXml(xmlFile);
	}

	void setTagFloatValue(string _path, int _index, float _val, bool _autoSave = true) {
		xml.find(_path)[_index].set(_val);
		if (_autoSave) writeXml(xmlFile);
	}

	void writeXml(string _fileName) {
		xml.save(_fileName);
	}
    
    void drawFPS(ofxFilikaAlignment _position, int _x = 0, int _y = 0) {
        int x = 0;
        int y = 0;
        
        string fr = ofToString(ofGetFrameRate(),1);
        
        switch(_position) {
            case TOP_LEFT:
                x = 0;
                y = bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case TOP_RIGHT:
                x = ofGetWidth() - bf.getBoundingBox(fr, 0, 0).getWidth();
                y = bf.getBoundingBox(fr, 0, 0).getHeight();;
                break;
            case BOTTOM_RIGHT:
                x = ofGetWidth() - bf.getBoundingBox(fr, 0, 0).getWidth();
                y = ofGetHeight() - bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case BOTTOM_LEFT:
                x = 0;
                y = ofGetHeight() - bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case TOP_MIDDLE:
                x = ofGetWidth() * 0.5 - bf.getBoundingBox(fr, 0, 0).getWidth() * 0.5;
                y = bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case CENTER:
                x = ofGetWidth() * 0.5 - bf.getBoundingBox(fr, 0, 0).getWidth() * 0.5;
                y = ofGetHeight() * 0.5 - bf.getBoundingBox(fr, 0, 0).getHeight() * 0.5;
                break;
            case CENTER_LEFT:
                x = 0;
                y = ofGetHeight() * 0.5 - bf.getBoundingBox(fr, 0, 0).getHeight() * 0.5;
                break;
            case CENTER_RIGHT:
                x = ofGetWidth() - bf.getBoundingBox(fr, 0, 0).getWidth();
                y = ofGetHeight() * 0.5 - bf.getBoundingBox(fr, 0, 0).getHeight() * 0.5;
                break;
            case BOTTOM_MIDDLE:
                x = ofGetWidth() * 0.5 - bf.getBoundingBox(fr, 0, 0).getWidth() * 0.5;
                y = ofGetHeight() - bf.getBoundingBox(fr, 0, 0).getHeight();
                break;
            case CUSTOM_POS:
                x = _x;
                y = _y;
                break;
        }
        
        ofDrawBitmapStringHighlight(fr, x, y);
    }
    bool notifyPerSecond(float timePeriod) {
        bool result = false;
        float now = ofGetElapsedTimef();
        if(now > nextEventSeconds) {
                // do something here that should only happen
                // every 3 seconds
            nextEventSeconds = now + timePeriod;
            result = true;
        }
        
        return result;
    }
    
    ofTrueTypeFont & getTurkishFont(ofTrueTypeFont & font, string fontName, int fontSize) {
        
        ofTrueTypeFontSettings settings(fontName, fontSize);
        
        settings.antialiased = true;
        settings.dpi = 72;
        settings.addRange(ofUnicode::Latin);
        settings.addRange(ofUnicode::LatinA);
        settings.addRange(ofUnicode::Latin1Supplement);
        font.load(settings);
        
        return font;
    }

	string ofxGetDateCompact()
	{
		stringstream returnDate;

		// Year
		returnDate << ofToString(ofGetYear()) + "_";
		// LEADING ZEROS
		// Month
		returnDate << setw(2) << setfill('0') << ofGetMonth();
		returnDate << "_";
		// Day
		returnDate << setw(2) << setfill('0') << ofGetDay();
		returnDate << "-";
		// Hour
		returnDate << setw(2) << setfill('0') << ofGetHours();
		returnDate << "_";
		// Minutes
		returnDate << setw(2) << setfill('0') << ofGetMinutes();
		returnDate << "_";
		// Seconds
		returnDate << setw(2) << setfill('0') << ofGetSeconds();

		return returnDate.str();
	}


	string ofxGetMonthDayCompact()
	{
		stringstream returnDate;

		// Month
		returnDate << setw(2) << setfill('0') << ofGetMonth();
		returnDate << "_";
		// Day
		returnDate << setw(2) << setfill('0') << ofGetDay();

		return returnDate.str();
	}

	string ofxGetLeadingZeros(const string & value, int numberOfZeros)
	{
		stringstream returnDate;

		returnDate << setw(numberOfZeros) << setfill('0') << value;

		return returnDate.str();
	}

	string ofxGetLeadingZeros(const int & value, int numberOfZeros)
	{
		return ofxGetLeadingZeros(ofToString(value), numberOfZeros);
	}

	ofPoint ofxFitToScreen(const int & width, const int & height, const int & screenWidth, const int & screenHeight, bool upScale)
	{
		ofPoint returnSize;

		float windowRatio = (float)screenWidth / screenHeight;
		float frameRatio = (float)width / height;

		if (!upScale && screenWidth > width && screenHeight > height)
		{
			returnSize = ofPoint(width, height);
			// ofLogNotice("ofxUtils", "upScale is set to false, so no change.");
			return returnSize;
		}

		else
		{
			if (windowRatio > frameRatio)
			{
				returnSize.x = (int)screenHeight * frameRatio;
				returnSize.y = screenHeight;
			}

			else if (windowRatio < frameRatio)
			{
				returnSize.x = screenWidth;
				returnSize.y = (int)screenWidth / frameRatio;
			}

			else if (windowRatio == frameRatio)
			{
				returnSize.x = screenWidth;
				returnSize.y = screenHeight;
			}

			return returnSize;
		}
	}
};
