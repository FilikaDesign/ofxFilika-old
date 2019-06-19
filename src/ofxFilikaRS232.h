#pragma once
#include "ofMain.h"
#include "ofxSerial.h"

/* DEPENDENCIES */
/* ofxIO */
/* ofxPoco */
/* ofxSerial */
/* ofxGui */

class ofxFilikaRS232 
{
	private:
		/* PRIVATE VARIABLES */
		ofx::IO::SerialDevice device;
		//ofx::IO::PacketSerialDevice device;

		
		std::vector<ofx::IO::SerialDeviceInfo> devicesInfo;
		vector<string> responses;

		ofXml xml;

		ofx::IO::SerialDevice::DataBits databit;
		ofx::IO::SerialDevice::Parity parity;
		ofx::IO::SerialDevice::StopBits stopBit;
		ofx::IO::SerialDevice::FlowControl flowCtrl;


		string comPort;
		string dataBitsXml;
		string parityXml;
		string stopBitXml;
		string flowCtrlXml;
		string response;
		string serialDevPort;
		string serialDevPortIndex;

		int baudrate;
		
		bool isDebug;
		bool success;

	

		// GUI
		vector<ofParameter<void>> btnCommands;
		ofParameter<string> btnBaudrate;
		ofParameter<string> btnDataBitsXml;
		ofParameter<string> btnParityXml;
		ofParameter<string> btnStopBitXml;
		ofParameter<string> btnFlowCtrlXml;
		ofParameter<string> btnPortName;
	public:
		//Serial data notification
		//static ofEvent<string> SERIAL_RECEIVED;
		ofEvent<string> SERIAL_RECEIVED = ofEvent<string>();
		ofEvent<string> SERIAL_ERROR = ofEvent<string>();

		ofParameterGroup guiCommands;
		/* PUBLIC VARIABLES */
		/* SETTERS & GETTERS */
		std::vector<ofx::IO::SerialDeviceInfo> getDeviceList() {
			for (std::size_t i = 0; i < ofx::IO::SerialDeviceUtils::listDevices().size(); ++i)
			{
				cout << "RS232 Devices["<< i << "]" << ofx::IO::SerialDeviceUtils::listDevices()[i] << endl;
			}

			return ofx::IO::SerialDeviceUtils::listDevices();
		}

		string getDeviceComPort() {
			serialDevPort = "none";
			for (std::size_t i = 0; i < ofx::IO::SerialDeviceUtils::listDevices().size(); ++i)
			{
				//cout << "RS232 Devices["<< i << "]" << ofx::IO::SerialDeviceUtils::listDevices()[i] << endl;
				string serialDeviceName = ofSplitString(ofToString(ofx::IO::SerialDeviceUtils::listDevices()[i]), ", ")[2];
				if (serialDeviceName == comPort) {
					serialDevPort = ofSplitString(ofToString(ofx::IO::SerialDeviceUtils::listDevices()[i]), ", ")[0];
					break;
				}
			}
			return serialDevPort;

		}

		void setDeviceComPortIndex(string _comPort) {

			serialDevPortIndex = _comPort;

		}

		string getDeviceComPortIndex() {

			comPort = serialDevPortIndex;
			serialDevPort = comPort;

			return serialDevPortIndex;

		}

		void setDebug(bool _d) {
			isDebug = _d;
		}
		void setSettingsParameters(string _settingsPath) {
			xml.load(_settingsPath);

			//comPort		= xml.find("//root").getFirst().getChild("COM_PORT_NAME").getValue();
			baudrate	= ofToInt(xml.find("//root").getFirst().getChild("BAUDRATE").getValue());
			dataBitsXml = xml.find("//root").getFirst().getChild("DATALEN").getValue();  
			parityXml	= xml.find("//root").getFirst().getChild("PARITY_CHECK").getValue(); 
			stopBitXml	= xml.find("//root").getFirst().getChild("STOP_BIT").getValue();
			flowCtrlXml = xml.find("//root").getFirst().getChild("FLOW_CONTROL").getValue();

			// SET RESPONSES VECTOR
			int bNum = xml.find("//root/RESPONSES/RESPONSE").size();

			responses.resize(bNum);
			for (int i = 0; i < bNum; i++)
			{
				string bName = xml.find("//root/RESPONSES/RESPONSE")[i].getValue();
				responses[i] = bName;
				//cout << responses[i] << endl;
			}
			
			
			// SET DATABITS
			if (dataBitsXml == "DATA_BITS_EIGHT") {
				databit = ofx::IO::SerialDevice::DATA_BITS_EIGHT;
			}
			else if (dataBitsXml == "DATA_BITS_SEVEN") {
				databit = ofx::IO::SerialDevice::DATA_BITS_SEVEN;
			}
			else if (dataBitsXml == "DATA_BITS_SIX") {
				databit = ofx::IO::SerialDevice::DATA_BITS_SIX;
			}
			else if (dataBitsXml == "DATA_BITS_FIVE") {
				databit = ofx::IO::SerialDevice::DATA_BITS_FIVE;
			}
			else {
				databit = ofx::IO::SerialDevice::DATA_BITS_UNKNOWN;
			} 

			// SET PARITY
			if (parityXml == "PAR_NONE") {
				parity = ofx::IO::SerialDevice::PAR_NONE;
			}
			else if (parityXml == "PAR_ODD") {
				parity = ofx::IO::SerialDevice::PAR_ODD;
			}
			else if (parityXml == "PAR_EVEN") {
				parity = ofx::IO::SerialDevice::PAR_EVEN;
			}
			else if (parityXml == "PAR_MARK") {
				parity = ofx::IO::SerialDevice::PAR_MARK;
			}
			else if (parityXml == "PAR_SPACE") {
				parity = ofx::IO::SerialDevice::PAR_SPACE;
			}
			else if (parityXml == "PAR_UNKNOWN") {
				parity = ofx::IO::SerialDevice::PAR_UNKNOWN;
			}

			// SET STOPBITS
			if (stopBitXml == "STOP_ONE") {
				stopBit = ofx::IO::SerialDevice::STOP_ONE;
			}
			else if (stopBitXml == "STOP_ONE_POINT_FIVE") {
				stopBit = ofx::IO::SerialDevice::STOP_ONE_POINT_FIVE;
			}
			else if (stopBitXml == "STOP_TWO") {
				stopBit = ofx::IO::SerialDevice::STOP_TWO;
			}
			else if (stopBitXml == "STOP_UNKNOWN") {
				stopBit = ofx::IO::SerialDevice::STOP_UNKNOWN;
			}
		
			// SET FLOW CONTROL
			if (flowCtrlXml == "FLOW_CTRL_NONE") {
				flowCtrl = ofx::IO::SerialDevice::FLOW_CTRL_NONE;
			}
			else if (flowCtrlXml == "FLOW_CTRL_HARDWARE") {
				flowCtrl = ofx::IO::SerialDevice::FLOW_CTRL_HARDWARE;
			}
			else if (flowCtrlXml == "FLOW_CTRL_SOFTWARE") {
				flowCtrl = ofx::IO::SerialDevice::FLOW_CTRL_SOFTWARE;
			}
			else if (flowCtrlXml == "FLOW_CTRL_UNKNOWN") {
				flowCtrl = ofx::IO::SerialDevice::FLOW_CTRL_UNKNOWN;
			}

			//cout << "xml bitti mi " << endl;

		}
		void getResponseSamsungPMF55() {

			try
			{
				// Read all bytes from the device;
				uint8_t buffer[1024];

				while (device.available() > 0)
				{
					std::size_t sz = device.readBytes(buffer, 1024);

					

					for (std::size_t i = 0; i < sz; ++i)
					{
						response += "0x" + ofToUpper(ofToHex(buffer[i]));
						if (i != sz - 1) {
							response += ",";
						}
					}

					

					//response = "";
					//so now lets search in the xml
					int responseXMLLength = 39;
					if (response.length() >= responseXMLLength) {
						for (int i = 0; i < responses.size(); i++)
						{

							if (ofIsStringInString(responses[i], response)) {
								string bName = xml.find("//root/RESPONSES/RESPONSE")[i].getAttribute("resp").getValue();
								ofNotifyEvent(SERIAL_RECEIVED, bName);
								break;
							}
						}

						response = "";

					}
					
					
					/*
					for (int i = 0; i < responses.size(); i++)
					{
						//string bName = xml.find("//root/RESPONSES/RESPONSE")[i].getValue();
						//responses[i] = bName;
						if (responses[i] == response) {
							string bName = xml.find("//root/RESPONSES/RESPONSE")[i].getAttribute("resp").getValue();
							ofNotifyEvent(SERIAL_RECEIVED, bName);
							
							break;
						}
					}
					*/

				}
			}
			catch (const std::exception& exc)
			{
				ofLogError("ofApp::update") << exc.what();
			}

		}

		void getResponseBenQ() {
			
			try
			{
				// Read all bytes from the device;
				uint8_t buffer[1024];
				
				while (device.available() > 0)
				{
					std::size_t sz = device.readBytes(buffer, 1024);

					bool endofSerialResponse = false;

					for (std::size_t i = 0; i < sz; ++i)
					{
						response += "0x" + ofToUpper(ofToHex(buffer[i]));
						response += ",";
					}
					
					endofSerialResponse = ofIsStringInString(response, ",0x23,0x0D,0x0A");


					//cout << response.find_last_of("0x23", 4) << endl;
					
					string splicedResponse = "";

					if (endofSerialResponse) {
						if (response.back() == ',') {
							splicedResponse = response.substr(0, response.size() - 1);
						}
						//cout << "splicedResponse : " << splicedResponse  << " : " << endofSerialResponse << endl;
						response = "";

						
			
					}
					
					//so now lets search in the xml
					for (int i = 0; i < responses.size(); i++)
					{
						//string bName = xml.find("//root/RESPONSES/RESPONSE")[i].getValue();
						//responses[i] = bName;
						//if (responses[i] == splicedResponse) {
						if(ofIsStringInString(splicedResponse, responses[i])){
							string bName = xml.find("//root/RESPONSES/RESPONSE")[i].getAttribute("resp").getValue();
							ofNotifyEvent(SERIAL_RECEIVED, bName);
							//cout << bName << endl;
							//cout << "Response: " << response << endl;
							//response = "";

							break;
						}
					}
					
					
					

					//cout << "splicedResponse: " << splicedResponse << endl;

					

				
				}
			}
			catch (const std::exception& exc)
			{
				ofLogError("ofApp::update") << exc.what();
			}

		}
		void setup(string _configFile) {

			devicesInfo = getDeviceList();
			
			response = "";
			
			if (!devicesInfo.empty())
			{
				// set Related parameters
				setSettingsParameters(_configFile);
				
			
				// Connect to the device.

				///comPort
				cout <<  getDeviceComPortIndex() << endl;
				success = device.setup(getDeviceComPortIndex(), baudrate, databit, parity, stopBit, flowCtrl);
				initGUI();
				if (success)
				{
					
				}
				else
				{
					cout << "ofxFilikaRS232::setup()" << " Unable to setup " << comPort << endl;
					string bName = "ERROR_001_RS232_CONNECTION";
					ofNotifyEvent(SERIAL_ERROR, bName);
				}
			}
			else
			{
				cout << "ofxFilikaRS232::setup()" << " No devices connected." << endl;
			}
		}

		
		void initGUI() {
			// Init GUI
			guiCommands.setName("RS232 Controls");
			if (!success) {
				guiCommands.add(btnPortName.set("CONNECTION FAIL"));
			}
			else {
				guiCommands.add(btnPortName.set("DevicePort:", serialDevPort));
				guiCommands.add(btnDataBitsXml.set("DataBits:", dataBitsXml));
				guiCommands.add(btnBaudrate.set("Baudrate:", ofToString(baudrate)));
				guiCommands.add(btnParityXml.set("Parity:", parityXml));
				guiCommands.add(btnStopBitXml.set("StopBits:", stopBitXml));
				guiCommands.add(btnFlowCtrlXml.set("FlowCtrl:", flowCtrlXml));

				int bNum = xml.find("//root/COMMANDS/COMMAND").size();

				btnCommands.resize(bNum);
				for (int i = 0; i < bNum; i++)
				{
					string bName = xml.find("//root/COMMANDS/COMMAND")[i].getAttribute("func").getValue();
					btnCommands[i].setName(bName);
					guiCommands.add(btnCommands[i].set(bName));
					btnCommands[i].addListener(this, &ofxFilikaRS232::bHandler);
				}
			}
			

			
			
		
		}

		void bHandler(const void * sender) {
			ofParameter<void> * button = (ofParameter<void>*)sender;
			string nn = button->getName();
			//cout << "test " <<  nn << endl;
			sendCommand(nn);
		}

		void sendCommand(string _CMD) {

			

			vector<int> command;

			// Loop through Button number to check attributes
			for (int i = 0; i < btnCommands.size(); i++)
			{
				if (xml.find("//root/COMMANDS/COMMAND")[i].getValue() != "") {
					
					if (xml.find("//root/COMMANDS/COMMAND")[i].getAttribute("func").getValue() == _CMD) {
						string mrec = xml.find("//root/COMMANDS/COMMAND")[i].getValue();
						vector<string> mrecs = ofSplitString(mrec, ",");
						
						//the command can be either in hex format or string
						
						if (mrecs.size() == 1) {
							//String sequence
							string outgoing = ofToString(mrec) + "\r";
							
							unsigned char* outgoingCharArray = new unsigned char[outgoing.size()];
							memcpy(outgoingCharArray, outgoing.c_str(), outgoing.size());

							device.writeBytes(outgoingCharArray, outgoing.size());
						}
						else {
							//Hex Sequence
							for (size_t i = 0; i < mrecs.size(); i++)
							{
								unsigned int t = std::stoul(mrecs[i], nullptr, 16);
								command.push_back(t);
							}

							// Execute rs232 command
							if (command.size() > 0)
							{
								try
								{
									
									for (int i = 0; i < command.size(); i++) {
										//cout << command[i] << endl;
										device.writeByte(command[i]); // command[i]
									}
								}
								catch (const std::exception& exc)
								{
									cout << exc.what() << endl;
								}
							}
						}
						
					}
				}
			}


			/*if (xml.find("//root").getFirst().getChild(_CMD).getValue() != "") {
				//cout << "message: " << ofToString(xml.get(_CMD)) << endl;

				string mrec = xml.find("//root").getFirst().getChild(_CMD).getValue();
				vector<string> mrecs = ofSplitString(mrec, ",");

				//cout << mrecs.size() << endl;

				for (size_t i = 0; i < mrecs.size(); i++)
				{
					unsigned int t = std::stoul(mrecs[i], nullptr, 16);
					command.push_back(t);
				}
				
			}*/
		}
};
