#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#endif

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxFilikaMultiTouchEvents.h"



//touch
static float _tempDist;
static float _curDist;
static float _scale = 1, _tempScale = 1, angelRot = 0;
static bool _gestureBegin;

static WNDPROC prevWndProc;
/* Get window pointer */

// MULTI TOUCH EVENT ARGS
ofEvent<ofxFilikaZoomEvent> OFX_FILIKA_ZOOM_EVENT;
ofEvent<ofxFilikaRotationEvent> OFX_FILIKA_ROTATION_EVENT;
ofEvent<void> OFX_FILIKA_GESTURE_BEGIN_EVENT;
ofEvent<void> OFX_FILIKA_GESTURE_END_EVENT;

double getPinchScale(float _dist)
{
	_curDist = _dist;

	_scale = (_curDist / _tempDist) * _tempScale;

	if (_scale > 4) {
		_scale = 4;
	}

	if (_scale < 1) {
		_scale = 1;
	}

	return _scale;
}

double rotationToRad(UINT arg)
{
	return ((((double)(arg) / 65535.0) * 4.0 * 3.14159265) - 2.0 * 3.14159265);
}

struct EnumWindowsCallbackArgs {
	EnumWindowsCallbackArgs(DWORD p) : pid(p) { }
	const DWORD pid;
	std::vector<HWND> handles;
};

static BOOL CALLBACK EnumWindowsCallback(HWND hnd, LPARAM lParam) {
	EnumWindowsCallbackArgs *args = (EnumWindowsCallbackArgs *)lParam;

	DWORD windowPID;
	(void)::GetWindowThreadProcessId(hnd, &windowPID);
	if (windowPID == args->pid) {
		args->handles.push_back(hnd);
	}

	return TRUE;
}

static std::vector<HWND> getToplevelWindows() {
	EnumWindowsCallbackArgs args(::GetCurrentProcessId());
	if (::EnumWindows(&EnumWindowsCallback, (LPARAM)&args) == FALSE) {
		return std::vector<HWND>();
	}
	return args.handles;
}

static HWND getOfxWindow() {
	HWND hwnd = ofGetWin32Window();
	if (!hwnd) {
		/* Glut or something */
		vector<HWND> windows = getToplevelWindows();
		HWND consoleWindow = GetConsoleWindow();
		for (HWND h : windows) {
			if (h != consoleWindow) {
				hwnd = h;
				break;
			}
		}
	}
	return hwnd;
}

/* Handle incoming pointer event */
static bool handlePointerEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	UINT32 pointerId = GET_POINTERID_WPARAM(wParam);
	POINTER_INPUT_TYPE pointerType = PT_POINTER;

	if (!GetPointerType(pointerId, &pointerType)) {
		ofLogError() << "GetPointerType failed: 0x" << hex << GetLastError();
		pointerType = PT_POINTER; // default to PT_POINTER
	}

	ofTouchEventArgs touch;

	union {
		POINTER_INFO pointer;
		POINTER_TOUCH_INFO touch;
		POINTER_PEN_INFO pen;
	} info;
	POINTER_INFO *pointer = &info.pointer;

	switch (pointerType) {
	case PT_TOUCH: {
		if (!GetPointerTouchInfo(pointerId, &info.touch)) {
			ofLogError() << "GetPointerTouchInfo failed: 0x" << hex << GetLastError();
			return false;
		}
		pointer = &info.touch.pointerInfo;
		touch.id = pointer->pointerId;
		touch.width = info.touch.rcContact.right - info.touch.rcContact.left;
		touch.height = info.touch.rcContact.bottom - info.touch.rcContact.top;
		touch.angle = info.touch.orientation; // degrees 0-359
		touch.pressure = info.touch.pressure; // 0-1024
		touch.majoraxis = max(touch.width, touch.height);
		touch.minoraxis = min(touch.width, touch.height);
		break;
	}
	case PT_PEN: {
		if (!GetPointerPenInfo(pointerId, &info.pen)) {
			ofLogError() << "GetPointerPenInfo failed: 0x" << hex << GetLastError();
			return false;
		}
		pointer = &info.pen.pointerInfo;

		touch.width = touch.height = 0;
		touch.angle = info.pen.rotation; // degrees 0-359
		touch.pressure = info.pen.pressure; // 0-1024
		touch.majoraxis = touch.minoraxis = 0;

		break;
	}
	default:
		return false; /* unsupported pointer type */
	}

	/* Use HIMETRIC positions for improved precision (subpixel) */
	RECT himetricRect, displayRect;
	if (GetPointerDeviceRects(pointer->sourceDevice, &himetricRect, &displayRect)
		&& himetricRect.right - himetricRect.left > 0
		&& himetricRect.bottom - himetricRect.top > 0) {
		double x = ((double)pointer->ptHimetricLocation.x - himetricRect.left) / (himetricRect.right - himetricRect.left);
		double y = ((double)pointer->ptHimetricLocation.y - himetricRect.top) / (himetricRect.bottom - himetricRect.top);
		touch.x = displayRect.left + x * (displayRect.right - displayRect.left) - ofGetWindowPositionX();
		touch.y = displayRect.top + y * (displayRect.bottom - displayRect.top) - ofGetWindowPositionY();
	}
	else {
		touch.x = pointer->ptPixelLocation.x - ofGetWindowPositionX();
		touch.y = pointer->ptPixelLocation.y - ofGetWindowPositionY();
	}
	touch.id = pointer->pointerId;
	if (pointer->pointerFlags & (POINTER_FLAG_CANCELED | POINTER_FLAG_CAPTURECHANGED)) {
		/* Cancelled */
		touch.type = ofTouchEventArgs::Type::cancel;
		ofNotifyEvent(ofEvents().touchCancelled, touch);
		return true;
	}

	/*
	if(!(pointer->pointerFlags & POINTER_FLAG_CONFIDENCE)) {
		ofLogError() << "pointer id " << touch.id << " not confident: flags 0x" << hex << pointer->pointerFlags;
	}
	*/
	
	if (pointer->pointerFlags & POINTER_FLAG_DOWN) {
		touch.type = ofTouchEventArgs::Type::down;
		ofNotifyEvent(ofEvents().touchDown, touch);
	}
	if (pointer->pointerFlags & POINTER_FLAG_UPDATE) {
		touch.type = ofTouchEventArgs::Type::move;
		ofNotifyEvent(ofEvents().touchMoved, touch);
	}
	if (pointer->pointerFlags & POINTER_FLAG_UP) {
		touch.type = ofTouchEventArgs::Type::up;
		ofNotifyEvent(ofEvents().touchUp, touch);
	}
	return true;
}



LRESULT DecodeGesture(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	//UnregisterTouchWindow(hWnd);

	// Create a structure to populate and retrieve the extra message info.
	GESTUREINFO gi;

	ZeroMemory(&gi, sizeof(GESTUREINFO));

	gi.cbSize = sizeof(GESTUREINFO);

	BOOL bResult = GetGestureInfo((HGESTUREINFO)lParam, &gi);
	BOOL bHandled = FALSE;

	ofxFilikaZoomEvent zoomGesture;
	ofxFilikaRotationEvent rotationGesture;

	if (bResult) {
		// now interpret the gesture

		switch (gi.dwID) {

		case GID_BEGIN:
			//cout << "gesture begin" << endl;
			ofNotifyEvent(OFX_FILIKA_GESTURE_BEGIN_EVENT);
			break;

		case GID_END:
			//cout << "gesture end" << endl;
			ofNotifyEvent(OFX_FILIKA_GESTURE_END_EVENT);
			_gestureBegin = false;

			_tempScale = _scale;
			break;

		case GID_ZOOM:
			// Code for zooming goes here     
			bHandled = TRUE;

			if (!_gestureBegin) {
				_tempDist = gi.ullArguments;
				_gestureBegin = true;
			}

			//cout << "x: " << gi.ptsLocation.x - ofGetWindowPositionX() << endl;
			//cout << "y: " << gi.ptsLocation.y - ofGetWindowPositionY() << endl;

			//cout << "ZOOM x:y " << gi.ptsLocation.x << ":" << gi.ptsLocation.y << endl;
			
			zoomGesture.center = glm::vec2(gi.ptsLocation.x - ofGetWindowPositionX(), gi.ptsLocation.y - ofGetWindowPositionY());
			zoomGesture.distance = gi.ullArguments;
			zoomGesture.scale = getPinchScale(gi.ullArguments);

			ofNotifyEvent(OFX_FILIKA_ZOOM_EVENT, zoomGesture);

			break;

		case GID_PAN:
			// Code for panning goes here
			//cout << "panning" << endl;
			bHandled = TRUE;
			break;
		case GID_ROTATE:
			// Code for rotation goes here
			//angelRot = ArgToRadians(gi.ullArguments);
			//cout << "Rotation center " << gi.ptsLocation.x << ":" << gi.ptsLocation.y << endl;
			//cout << ArgToRadians(gi.ullArguments) << endl;

			rotationGesture.center = glm::vec2(gi.ptsLocation.x - ofGetWindowPositionX(), gi.ptsLocation.y - ofGetWindowPositionY());
			rotationGesture.rotation = rotationToRad(gi.ullArguments);

			//ofNotifyEvent(OFX_FILIKA_ROTATION_EVENT, rotationGesture);

			bHandled = TRUE;
			break;
		case GID_TWOFINGERTAP:
			// Code for two-finger tap goes here
			bHandled = TRUE;
			break;
		case GID_PRESSANDTAP:
			// Code for roll over goes here
			bHandled = TRUE;
			break;
		default:
			// A gesture was not recognized
			break;
		}
	}
	else {
		DWORD dwErr = GetLastError();
		if (dwErr > 0) {
			//MessageBoxW(hWnd, L"Error!", L"Could not retrieve a GESTUREINFO structure.", MB_OK);
		}
	}
	if (bHandled) {
		return 0;
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}





LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_GESTURE:
		// Insert handler code here to interpret the gesture.            
		return DecodeGesture(hWnd, message, wParam, lParam);

	}
}

/* Window callback function */
LRESULT APIENTRY pointerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	//gesture recognition
	DecodeGesture(hwnd, uMsg, wParam, lParam);

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg) {
	case WM_POINTERUPDATE:
		if (!IS_POINTER_INCONTACT_WPARAM(wParam))
			break;
		/* fall through */
	case WM_POINTERDOWN:
	case WM_POINTERUP:
	case WM_POINTERCAPTURECHANGED:
		if (handlePointerEvent(uMsg, wParam, lParam))
			return 0;
		break; // fail, call CallWindowProc
	}
	// Chain to the next wndproc
	return CallWindowProc(prevWndProc, hwnd, uMsg, wParam, lParam);
}

void ofxFilikaMultiTouchSetup() {


	HWND hwnd = getOfxWindow();
	if (!hwnd) {
		ofLogError() << "Failed to get HWND for ofx window - try calling this later in update().";
		return;
	}

	if (prevWndProc) {
		ofLogError() << "Already called ofxWin8TouchSetup!";
		return;
	}

	//prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pointerWndProc);

	// to enable rotation
	/*
	// set the settings in the gesture configuration
	GESTURECONFIG gc[] = { { GID_ZOOM, GC_ZOOM, 0 },
						  { GID_ROTATE, GC_ROTATE, 0},
						  { GID_PAN, GC_PAN , 0},
						  { GID_TWOFINGERTAP, GC_TWOFINGERTAP , 0},
						  { GID_PRESSANDTAP, GC_PRESSANDTAP , 0}
	};

	UINT uiGcs = 5;
	BOOL bResult = SetGestureConfig(hwnd, 0, uiGcs, gc, sizeof(GESTURECONFIG));
	
	if (!bResult) {
		DWORD err = GetLastError();
	}

	*/
	prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pointerWndProc);
}

/*
GID_BEGIN	1	A gesture is starting.
GID_END	2	A gesture is ending.
GID_ZOOM	3	The zoom gesture.
GID_PAN	4	The pan gesture.
GID_ROTATE	5	The rotation gesture.
GID_TWOFINGERTAP	6	The two - finger tap gesture.
GID_PRESSANDTAP	7	The press and tap gesture.
*/

//https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setgestureconfig
//https://github.com/pauldotknopf/WindowsSDK7-Samples/blob/master/Touch/MTGestures/CS/MTGestures.cs
//https://docs.microsoft.com/en-us/windows/desktop/wintouch/wm-gesture
