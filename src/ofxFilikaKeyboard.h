#pragma once
#include "ofMain.h"
#include "ofxFilikaKeys.h"
#include "ofxFilikaUtils.h"
#include "ofxFilikaKeyboard_InputField.h"
#include "ofxFilikaEvents.h"

class ofxFilikaKeyboard:public ofxFilikaEvents{
public:
	int getWidth() {
		return this->w;

	}
	string getText() {
		return drawingText;
	}
	void setActive(bool state) {
		active = state;
		inputField.setActive(state);
	}
	void setKeyActive(string key, bool state) {
		for (int i = 0; i < keys.size(); i++) {
			if (key == keys[i].getKey()) {
				keys[i].setActive(state);
			}
		}
	}
	void setKeyboardUppercase() {
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setUppercase();
		}
	}
	void setKeyboardLowercase() {
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setLowercase();
		}
	}
	void setButtonRoundness(float buttonRoundness) {
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setRoundness(buttonRoundness);
		}
	}
	void setKeyTextSize(float size) {
		fontKeySize = size;
	}
	void setInputFieldTextSize(float size) {
		fontOutputSize = size;
	}
	void setInputFieldArea(int x, int y, int width, int height) {
		inputField.setArea(x, y, width, height);
	}
	void setInputFieldPadding(float padding) {
		inputField.setPadding(padding);
	}
	void setRoundness(float roundness) {
		this->roundness = roundness;
	}
	void setMargin(float margin) {
		this->margin = margin;
		removeKeyListeners();
		loadKeys();
		addKeyListeners();
	}
	void setColorBase(ofColor colorBase) {
		this->colorBase = colorBase;
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setColorBase(colorBase);
		}
	}
	void setColorButton(ofColor colorButton) {
		this->colorButton = colorButton;
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setColorButton(colorButton);
		}
	}
	void setColorText(ofColor colorText) {
		this->colorText = colorText;
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setColorText(colorText);
		}
	}
	void setColorButtonPressed(ofColor colorButtonPressed) {
		this->colorButtonPressed = colorButtonPressed;
		for (int i = 0; i < keys.size(); i++) {
			keys[i].setColorButtonPressed(colorButtonPressed);
		}
	}
	void disableInteraction() {
		removeKeyListeners();
		ofRemoveListener(inputField.INPUT_FIELD_CLICKED, this, &ofxFilikaKeyboard::inputActivated);
	}
	void enableInteraction() {
		addKeyListeners();
		ofAddListener(inputField.INPUT_FIELD_CLICKED, this, &ofxFilikaKeyboard::inputActivated);
	}

	void setup(int x, int y, int w, string fontPath) {
		fontKeySize = w*0.03f;
		fontOutputSize = w * 0.05f;
		initFont(fontPath);
		this->active = false;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = (float)w * 368.0f / 1072.0f;
		this->roundness = 0;
		this->margin = (float)w * 0.01f;
		this->text.clear();
		colorBase = ofColor(54, 71, 79);
		loadKeys();
		inputField.setup(x, y*0.5f, w, h*0.3f, h*0.1f, *fontOutput, colorText);
	}
	void display() {
		ofPushStyle();
		if (active) {
			inputField.draw();
			ofSetColor(colorBase);
			// Num pad space
			ofDrawRectRounded(x, (y - h * 0.5) - margin * 0.5, w, h / 2, roundness);
			ofDrawRectRounded(x, y, w, h, roundness);
			drawKeys();
		}
		ofPopStyle();
	}
private:
	vector<ofxFilikaKeys> keys;
	ofxFilikaKeyboard_InputField inputField;
	bool active;
	int x, y, w, h; //inputFieldX, inputFieldY, inputFieldW, inputFieldH, inputFieldPadding;
	float margin, fontKeySize, fontOutputSize, roundness;
	ofColor colorBase, colorButton, colorText, colorButtonPressed;
	ofTrueTypeFont *fontKey, *fontOutput;
	vector<string> text;
	ofxFilikaUtils utils;
	string drawingText;

	void drawKeys() {
		for (int i = 0; i < keys.size(); i++) {
			keys[i].draw();
		}
	}
	void removeKeyListeners() {
		for (int i = 0; i < keys.size(); i++) {
			ofRemoveListener(keys[i].KEY_PRESSED, this, &ofxFilikaKeyboard::keyPressed);
		}
	}
	void addKeyListeners() {
		for (int i = 0; i < keys.size(); i++) {
			ofAddListener(keys[i].KEY_PRESSED, this, &ofxFilikaKeyboard::keyPressed);
		}
	}
	void inputActivated() {
		active = true;
	}
	void keyPressed(string &key) {
		if (active) {
			string ext = utils.getFileExtension(key);
			if (ext != "") {
				if (key == "assets/delete.png" && text.size() > 0) {
					text.pop_back();
				}
				else if (key == "assets/shift_close.png") {
					setKeyboardUppercase();
				}
				else if (key == "assets/shift_open.png") {
					setKeyboardLowercase();
				}
				else if (key == "assets/enter.png") {
					setActive(false);

					// ENTER Key pressed
					ofNotifyEvent(ON_TEXT_RETURN, drawingText, this);
				}
			}
			else {
				if (key == "GÖNDER") {
					setActive(false);

					// ENTER Key pressed
					ofNotifyEvent(ON_TEXT_RETURN, drawingText, this);
				}
				else {
					text.push_back(key);
				}
			}

			drawingText = "";
			for (int i = 0; i < text.size(); i++) {
				drawingText += text[i];
			}
			inputField.setDrawingText(drawingText);
		}
	}
	void loadKeys() {
		keys.clear();
		keys.resize(65);
		float keySize = ((float)w - margin * 13) / 12;

		keys[0].setup("!", "!", *fontKey, x + margin * 1 + keySize * 0, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[1].setup("'", "'", *fontKey, x + margin * 2 + keySize * 1, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[2].setup("#", "#", *fontKey, x + margin * 3 + keySize * 2, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[3].setup("$", "$", *fontKey, x + margin * 4 + keySize * 3, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[4].setup("%", "%", *fontKey, x + margin * 5 + keySize * 4, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[5].setup("&", "&", *fontKey, x + margin * 6 + keySize * 5, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[6].setup("/", "/", *fontKey, x + margin * 7 + keySize * 6, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[7].setup("{", "{", *fontKey, x + margin * 8 + keySize * 7, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[8].setup("}", "}", *fontKey, x + margin * 9 + keySize * 8, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[9].setup("+", "+", *fontKey, x + margin * 10 + keySize * 9, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[10].setup("-", "-", *fontKey, x + margin * 11 + keySize * 10, y - margin * 2 - keySize * 2, keySize, keySize);
		keys[11].setup("=", "=", *fontKey, x + margin * 12 + keySize * 11, y - margin * 2 - keySize * 2, keySize, keySize);

		keys[12].setup("1", "1", *fontKey, x + margin * 1 + keySize * 0, y - margin - keySize, keySize, keySize);
		keys[13].setup("2", "2", *fontKey, x + margin * 2 + keySize * 1, y - margin - keySize, keySize, keySize);
		keys[14].setup("3", "3", *fontKey, x + margin * 3 + keySize * 2, y - margin - keySize, keySize, keySize);
		keys[15].setup("4", "4", *fontKey, x + margin * 4 + keySize * 3, y - margin - keySize, keySize, keySize);
		keys[16].setup("5", "5", *fontKey, x + margin * 5 + keySize * 4, y - margin - keySize, keySize, keySize);
		keys[17].setup("6", "6", *fontKey, x + margin * 6 + keySize * 5, y - margin - keySize, keySize, keySize);
		keys[18].setup("7", "7", *fontKey, x + margin * 7 + keySize * 6, y - margin - keySize, keySize, keySize);
		keys[19].setup("8", "8", *fontKey, x + margin * 8 + keySize * 7, y - margin - keySize, keySize, keySize);
		keys[20].setup("9", "9", *fontKey, x + margin * 9 + keySize * 8, y - margin - keySize, keySize, keySize);
		keys[21].setup("0", "0", *fontKey, x + margin * 10 + keySize * 9, y - margin - keySize, keySize, keySize);
		keys[22].setup("*", "*", *fontKey, x + margin * 11 + keySize * 10, y - margin - keySize, keySize, keySize);
		keys[23].setup("_", "_", *fontKey, x + margin * 12 + keySize * 11, y - margin - keySize, keySize, keySize);


		
		keys[24].setup("q", "Q", *fontKey, x + margin * 1 + keySize * 0, y + margin, keySize, keySize);
		keys[25].setup("w", "W", *fontKey, x + margin * 2 + keySize * 1, y + margin, keySize, keySize);
		keys[26].setup("e", "E", *fontKey, x + margin * 3 + keySize * 2, y + margin, keySize, keySize);
		keys[27].setup("r", "R", *fontKey, x + margin * 4 + keySize * 3, y + margin, keySize, keySize);
		keys[28].setup("t", "T", *fontKey, x + margin * 5 + keySize * 4, y + margin, keySize, keySize);
		keys[29].setup("y", "Y", *fontKey, x + margin * 6 + keySize * 5, y + margin, keySize, keySize);
		keys[30].setup("u", "U", *fontKey, x + margin * 7 + keySize * 6, y + margin, keySize, keySize);
		keys[31].setup("ı", "I", *fontKey, x + margin * 8 + keySize * 7, y + margin, keySize, keySize);
		keys[32].setup("o", "O", *fontKey, x + margin * 9 + keySize * 8, y + margin, keySize, keySize);
		keys[33].setup("p", "P", *fontKey, x + margin * 10 + keySize * 9, y + margin, keySize, keySize);
		keys[34].setup("ğ", "Ğ", *fontKey, x + margin * 11 + keySize * 10, y + margin, keySize, keySize);
		keys[35].setup("ü", "Ü", *fontKey, x + margin * 12 + keySize * 11, y + margin, keySize, keySize);
		keys[36].setup("a", "A", *fontKey, x + margin * 1.5 + keySize * 0.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[37].setup("s", "S", *fontKey, x + margin * 2.5 + keySize * 1.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[38].setup("d", "D", *fontKey, x + margin * 3.5 + keySize * 2.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[39].setup("f", "F", *fontKey, x + margin * 4.5 + keySize * 3.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[40].setup("g", "G", *fontKey, x + margin * 5.5 + keySize * 4.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[41].setup("h", "H", *fontKey, x + margin * 6.5 + keySize * 5.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[42].setup("j", "J", *fontKey, x + margin * 7.5 + keySize * 6.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[43].setup("k", "K", *fontKey, x + margin * 8.5 + keySize * 7.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[44].setup("l", "L", *fontKey, x + margin * 9.5 + keySize * 8.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[45].setup("ş", "Ş", *fontKey, x + margin * 10.5 + keySize * 9.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[46].setup("i", "İ", *fontKey, x + margin * 11.5 + keySize * 10.5, y + margin * 2 + keySize * 1, keySize, keySize);
		keys[47].setup("z", "Z", *fontKey, x + margin * 2.5 + keySize * 1.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[48].setup("x", "X", *fontKey, x + margin * 3.5 + keySize * 2.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[49].setup("c", "C", *fontKey, x + margin * 4.5 + keySize * 3.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[50].setup("v", "V", *fontKey, x + margin * 5.5 + keySize * 4.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[51].setup("b", "B", *fontKey, x + margin * 6.5 + keySize * 5.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[52].setup("n", "N", *fontKey, x + margin * 7.5 + keySize * 6.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[53].setup("m", "M", *fontKey, x + margin * 8.5 + keySize * 7.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[54].setup("ö", "Ö", *fontKey, x + margin * 9.5 + keySize * 8.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[55].setup("ç", "Ç", *fontKey, x + margin * 10.5 + keySize * 9.5, y + margin * 3 + keySize * 2, keySize, keySize);
		keys[56].setup("assets/shift_close.png", "assets/shift_open.png", *fontKey, x + margin * 1 + keySize * 0, y + margin * 3 + keySize * 2, keySize * 1.5f + margin * 0.5f, keySize);
		keys[57].setup("assets/delete.png", "assets/delete.png", *fontKey, x + margin * 11.5 + keySize * 10.5, y + margin * 3 + keySize * 2, keySize * 1.5f + margin * 0.5f, keySize);
		keys[58].setup("@", "@", *fontKey, x + margin * 1 + keySize * 0, y + margin * 4 + keySize * 3, keySize, keySize);
		keys[59].setup(":", ":", *fontKey, x + margin * 2 + keySize * 1, y + margin * 4 + keySize * 3, keySize, keySize);
		keys[60].setup(";", ";", *fontKey, x + margin * 3 + keySize * 2, y + margin * 4 + keySize * 3, keySize, keySize);
		keys[61].setup(",", ",", *fontKey, x + margin * 4 + keySize * 3, y + margin * 4 + keySize * 3, keySize, keySize);
		keys[62].setup(" ", " ", *fontKey, x + margin * 5 + keySize * 4, y + margin * 4 + keySize * 3, keySize * 5 + margin * 4, keySize);
		keys[63].setup(".", ".", *fontKey, x + margin * 10 + keySize * 9, y + margin * 4 + keySize * 3, keySize, keySize);
		keys[64].setup("GÖNDER", "GÖNDER", *fontKey, x + margin * 11 + keySize * 10, y + margin * 4 + keySize * 3, keySize * 2 + margin, keySize);



		//?~
	}

	void initFont(string fontPath) {
		fontKey = new ofTrueTypeFont();
		ofTrueTypeFontSettings fontKeySettings(fontPath, fontKeySize);
		fontKeySettings.antialiased = true;
		fontKeySettings.dpi = 72;
		fontKeySettings.addRange(ofUnicode::Latin);
		fontKeySettings.addRange(ofUnicode::LatinA);
		fontKeySettings.addRange(ofUnicode::Latin1Supplement);
		fontKey->load(fontKeySettings);

		fontOutput = new ofTrueTypeFont();
		ofTrueTypeFontSettings fontOutputSettings(fontPath, fontOutputSize);
		fontOutputSettings.antialiased = true;
		fontOutputSettings.dpi = 72;
		fontOutputSettings.addRange(ofUnicode::Latin);
		fontOutputSettings.addRange(ofUnicode::LatinA);
		fontOutputSettings.addRange(ofUnicode::Latin1Supplement);
		fontOutput->load(fontOutputSettings);
	}
};