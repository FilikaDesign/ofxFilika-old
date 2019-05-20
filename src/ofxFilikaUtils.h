#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

class ofxFilikaUtils {
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
};
