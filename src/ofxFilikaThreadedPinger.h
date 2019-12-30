#pragma once

#include "ofMain.h"
#include "ofConstants.h"
#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

class ofxFilikaThreadedPinger : public ofThread {
public:
	/// Start the thread.
	void start() {
		startThread();
	}

	void stop() {
		std::unique_lock<std::mutex> lck(mutex);
		stopThread();
		//condition.notify_all();
	}

	~ofxFilikaThreadedPinger() {
		stop();
		toAnalyze.close();
		analyzed.close();
		waitForThread(true);
	}

	ofxFilikaThreadedPinger() :newFrame(true) {
		start();
	}


	bool isFrameNew() {
		return newFrame;
	}

	void threadedFunction() {

		string pixels;
		while (toAnalyze.receive(pixels)) {

			// The mutex is now locked so we can modify
			// the shared memory without problem
			auto t = ofGetElapsedTimef();
			HANDLE hIcmpFile;
			unsigned long ipaddr = INADDR_NONE;
			DWORD dwRetVal = 0;
			char SendData[32] = "Data Buffer";
			LPVOID ReplyBuffer = NULL;
			DWORD ReplySize = 0;


			ipaddr = inet_addr(pixels.c_str());
			if (ipaddr == INADDR_NONE) {
				if (verbose)
					printf("usage: %s IP address\n", pixels);
			}

			hIcmpFile = IcmpCreateFile();
			if (hIcmpFile == INVALID_HANDLE_VALUE) {
				if (verbose)
				{
					printf("\tUnable to open handle.\n");
					printf("IcmpCreatefile returned error: %ld\n", GetLastError());
				}
				result = false;
			}

			ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
			ReplyBuffer = (VOID*)malloc(ReplySize);
			if (ReplyBuffer == NULL) {
				if (verbose)
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
				if (verbose)
				{
					printf("\tSent icmp message to %s\n", pixels);
				}
				if (dwRetVal > 1) {
					if (verbose)
					{
						printf("\tReceived %ld icmp message responses\n", dwRetVal);
						printf("\tInformation from the first response:\n");
					}
				}
				else {
					if (verbose)
					{
						printf("\tReceived %ld icmp message response\n", dwRetVal);
						printf("\tInformation from this response:\n");
					}
				}
				if (verbose)
				{
					printf("\t  Received from %s\n", inet_ntoa(ReplyAddr));
					printf("\t  Status = %ld\n", pEchoReply->Status);
					printf("\t  Roundtrip time = %ld milliseconds\n", pEchoReply->RoundTripTime);
				}
				result = true;

			}
			else {
				if (verbose)
				{
					printf("\tCall to IcmpSendEcho failed.\n");
					printf("\tIcmpSendEcho returned error: %ld\n", GetLastError());
				}
				result = false;

			}
#if __cplusplus>=201103
			analyzed.send(std::move(pixels));
#else
			analyzed.send(pixels);
#endif
		}
	}

#ifdef WIN32
	bool pingIpAdd(string _argv, bool _verbose = false) {
		//start();
		argv = _argv;
		verbose = _verbose;
		//memcpy(argv, _argv, sizeof(char));

		

		return result;
		//stop();
	}
#endif

	void analyze(string & pixels) {
		// send the frame to the thread for analyzing
		// this makes a copy but we can't avoid it anyway if
		// we want to update the grabber while analyzing
		// previous frames
		toAnalyze.send(pixels);
	}

	void update() {
		newFrame = false;
		while (analyzed.tryReceive(argv)) {
			newFrame = true;
		}
		if (newFrame) {
			
		}
	}

	void updateNoLock() {
		// we don't lock here so we will see
		// tearing as the thread will update
		// the pixels while we upload them to the texture
		//condition.notify_all();
	}

	/// This drawing function cannot be called from the thread itself because
	/// it includes OpenGL calls
	void draw() {
	}

	int getThreadFrameNum() {
		return threadFrameNum;
	}

private:
	ofThreadChannel<string> toAnalyze;
	ofThreadChannel<string> analyzed;
	//std::condition_variable condition;
	int threadFrameNum = 0;

	bool result, verbose;
	bool newFrame;
	string  argv;
};