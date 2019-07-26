#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include "ofMain.h"


class ofxFilikaUtils {
private:
	ofXml xml;
	string xmlFile;

public:
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
};
