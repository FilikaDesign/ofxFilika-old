#pragma once
/* TYPE DEFINITIONS */
typedef enum {
	SHIFT,
	DEL,
	ENTER
} ofxFilikaKey;

typedef enum { 
	TR, 
	ENG, 
	ARABIC, 
	FR 
} Language;

/* ALIGNMENT */
typedef enum { 
	TOP_LEFT, 
	TOP_RIGHT, 
	TOP_MIDDLE, 
	CENTER, 
	CENTER_LEFT, 
	CENTER_RIGHT, 
	BOTTOM_LEFT, 
	BOTTOM_RIGHT, 
	BOTTOM_MIDDLE,
	CUSTOM_POS
} ofxFilikaAlignment;

/* Background shape mode for buttons 
   These type defs should be depreciated
*/
typedef enum  {
	NONE,
	CUSTOM,
	RECTANGLE,
	ELLIPSE,
	ROUNDRECT,
	IMAGE
} ofxFilikaBgMode;

/* Draggable button modes. Buttons could be either image or vector shape */
typedef enum  {
	BUTTON_MODE_IMAGE,
	BUTTON_MODE_CUSTOM,
	BUTTON_MODE_SHAPE_ROUNRECT,
	BUTTON_MODE_SHAPE_RECTANGLE,
	BUTTON_MODE_SHAPE_ELLIPSE
}ofxFilikaButtonMode;


typedef enum {
	SWIPE_UP,
	SWIPE_DOWN,
	SWIPE_LEFT,
	SWIPE_RIGHT
} ofxFilikaTouchGestures;