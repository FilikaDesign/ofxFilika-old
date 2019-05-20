#pragma once
#include "ofMain.h"
#include "ofxNetwork.h"

class device {
public:
	string name;
	//string broadcastIp;
	string macAdd;
	//int udpPort;
	string deviceIp;
	int oscSendPort;
	vector<string> subdevices;
};

class ofxFilikaLanDeviceControl
{
private:
	ofXml xml;
	vector<device> devices;

	ofxUDPManager udpConnection;
	string broadcastIp;
	int udpPort;

public:
	~ofxFilikaLanDeviceControl() {
		//cout << "closing connection" << endl;
		udpConnection.Close();
	}

	void setup(string _settings)
	{
		xml.load(_settings);
		// Parse XML
		broadcastIp = xml.find("//root").getFirst().getChild("BROADCAST_IP").getValue();
		udpPort		= ofToInt(xml.find("//root").getFirst().getChild("UDP_PORT").getValue());
		cout << broadcastIp <<"::"<< udpPort << endl;

		int listSize = xml.find("//root/DEVICE").size();
		devices.resize(listSize);


		for (int i = 0; i < listSize; i++)
		{
			devices[i].name				= xml.find("//root/DEVICE")[i].getChild("NAME").getValue();
			//devices[i].broadcastIp	    = xml.find("//root/DEVICE")[i].getChild("BROADCAST_IP").getValue();
			//devices[i].udpPort			= ofToInt(xml.find("//root/DEVICE")[i].getChild("UDP_PORT").getValue());
			devices[i].macAdd			= xml.find("//root/DEVICE")[i].getChild("MAC_ADD").getValue();
			devices[i].deviceIp			= xml.find("//root/DEVICE")[i].getChild("DEVICE_IP").getValue();
			devices[i].oscSendPort		= ofToInt(xml.find("//root/DEVICE")[i].getChild("OSC_SEND_PORT").getValue());

			int subListSize = xml.find("//root/DEVICE")[i].find("SUB_DEVICE").size();

			devices[i].subdevices.resize(subListSize);

			for (int j = 0; j < subListSize; j++) {
				devices[i].subdevices[j] = xml.find("//root/DEVICE")[i].find("SUB_DEVICE")[j].getChild("NAME").getValue();
			}
		}

		initializeUDPConnection();
	}

	void initializeUDPConnection() {
		// https://shadesfgray.wordpress.com/2010/12/17/wake-on-lan-how-to-tutorial/
		//create the socket and set to send to 127.0.0.1:11999
		ofxUDPSettings settings;
		settings.sendTo(broadcastIp, udpPort);
		settings.blocking = false;

		udpConnection.Setup(settings);
	}

	void wakeUp(int _deviceIndex) {
		unsigned char tosend[102];
		unsigned char mac[6];

		/** first 6 bytes of 255 **/
		for (int i = 0; i < 6; i++) {
			tosend[i] = 0xFF;
		}

		string _macAdd = devices[_deviceIndex].macAdd;

		
		vector<string> mrecs = ofSplitString(_macAdd, ":");

		for (size_t i = 0; i < mrecs.size(); i++)
		{
			unsigned int t = std::stoul(mrecs[i], nullptr, 16);
			mac[i] = t;
			//command.push_back(t);
		}
		/** store mac address 
		mac[0] = 0x00;
		mac[1] = 0x01;
		mac[2] = 0x2E;
		mac[3] = 0x4F;
		mac[4] = 0xDB;
		mac[5] = 0xF3;**/
		/** append it 16 times to packet **/
		for (int i = 1; i <= 16; i++) {
			memcpy(&tosend[i * 6], &mac, 6 * sizeof(unsigned char));
		}
		cout << sizeof(tosend) << endl;

		udpConnection.Send(reinterpret_cast<char*>(tosend), sizeof(tosend));
	}

};