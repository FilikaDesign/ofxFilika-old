#pragma once
#include "ofMain.h"

/*
Things to do before using the addon
1- Go to Settings.
2- Select Devices.
3- Click on Typing.
4- Turn on the option Automatically show the touch keyboard in windowed apps when there's no keyboard attached to your device.
*/

class ofxFilikaWinVirtualKeyboard : public ofThread {

public:
	ofxFilikaWinVirtualKeyboard() {
		//KeyboardLayoutPreference REG_DWORD [0~3] 0-QWERTY 1-thumb keyboard 2-handwriting 3-advance keyboard
		//SetTabletTipKey(_T("KeyboardLayoutPreference"), 3);
		//cout << "consoled run virtual keyboard constructor" << endl;


	}

	~ofxFilikaWinVirtualKeyboard() {
		//ofLog() << "Exit from Virtual Keyboard";
	}

	bool mode; //true opens false closes
	void startTouchKeyboard() {
		mode = true;
		startThread();

	}

	void stopTouchKeyboard() {
		mode = false;
		startThread();

	}


private:
	void threadedFunction() {
		if (mode) {
			ofSystem("C:\\Program\" \"Files\\Common\" \"Files\\microsoft\" \"shared\\ink\\TabTip.exe");
		}
		else {
			HWND hWnd = ::FindWindow(_T("IPTIP_Main_Window"), _T(""));
			::PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

			// Alp found the following command from 
			// https://github.com/LingliLiang/Miscellaneous/blob/f6786d3f71042e6ef57ebadd439c1642b4aa42f5/DuiTest/Utils/TabtipHandle.cpp
			//HWND hWnd = ::FindWindow(_TEXT("IPTip_Main_Window"), NULL);
			//::PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		stopThread();
	}

	void SetTabletTipKey(LPCTSTR key, DWORD kVaule)
	{
		HKEY hTabletTip = NULL;
		LSTATUS lres = 0;

		lres = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\TabletTip\\1.7"), 0, KEY_WRITE, &hTabletTip);
		if (lres == ERROR_SUCCESS)
		{
			lres = RegSetKeyValue(hTabletTip, NULL, key, REG_DWORD, (BYTE*)&kVaule, sizeof(kVaule));
			if (lres == ERROR_SUCCESS)
			{

			}
		}
		RegCloseKey(hTabletTip);
		cout << "consoled run virtual keyboard constructor" << endl;
	}

	
};