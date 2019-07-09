#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include "ofMain.h"

/* TYPE DEFINITIONS */
typedef enum { TR, ENG, ARABIC, FR } Language;

class ofxFilikaUtils {
private:
	ofXml xml;

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

	/* XML Operations */
	void loadXml(string _src) {
		xml.load(_src);
	}

	// should be called xml.find("//root/RESPONSES/RESPONSE").size();
	int getNumTags(string _tagPath) {
		return xml.find(_tagPath).size();
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
	string getTagValStr(string _tag) {
		return xml.find("//root").getFirst().getChild(_tag).getValue();
	}

	/* Second Level */
	string getTagValStr(string _tag, string _second_tag) {
		return xml.find("//root").getFirst().getChild(_tag).getChild(_second_tag).getValue();
	}
};
