//
//  ofxFilikaApp.h
//  rawAV2
//
//  Created by alp tugan on 1.04.2020.
//

#ifndef ofxFilikaApp_h
#define ofxFilikaApp_h

#include "ofAppRunner.h"
#include "ofConstants.h"
#include "ofColor.h"
#include "ofxGui.h"
#ifdef TARGET_OF_IOS
    #include "ofxiOS.h"
#else
    #include "ofBaseApp.h"
#endif

///    \class  App
///    \brief  application implementation with automatic transforms and projection mapping,
///            includes an optional built-in ofxControlPanel
///
///    simply inherit from this class and implement the regular app callbacks
///    ie setup(), update(), draw(), etc
class ofxFilikaApp :



#ifdef TARGET_OF_IOS
public ofxiOSApp {
#else
    public ofBaseApp {
#endif
    private:
        string settingsFile = "settings_main.json";
        ofParameter<void> btnSave;
        ofParameter<bool> gMinimizeAll;
        ofParameter<bool> gEnableMouseAutoHide = {"Enable/Disable Mouse AutoHide", false};
        ofParameter<bool> gWindowRatio = {"SET 1:1 RATIO", false};
        float idleDurationBeforeHiding;
        float lastTimeMouseMoved;
        bool cursorShowing;
        
        //--------------------------------------------------------------
        // METHOD: SAVE CURRENT SETTINGS
        //--------------------------------------------------------------
        void saveSettings() {
            ofxAppGui.saveToFile(settingsFile);
            ofLog() << "SETTINGS SAVED";
        }
        
        void setWindowRatio(bool & val) {
            if(val) {
                ofSetWindowShape(1080, 1080);
                ofLog() << "Window Raito -> 1:1";
            }else {
                ofSetWindowShape(1080, 607);
                ofLog() << "Window Raito -> 16:9";
            }
        }
        
        void minimizeMaximize(bool & _val) {
            if(_val) {
                ofxAppGui.minimizeAll();
            }else{
                ofxAppGui.maximizeAll();
            }
        }
        
    public:
        /* Declare variables here to access them directly from ofApp.cpp */
        ofxPanel ofxAppGui;
        
        ofxFilikaApp() {
            //ofAddListener(ofEvents().update, this, &ofxApp::onUpdate, OF_EVENT_ORDER_BEFORE_APP);
        }
        
        /*void onUpdate(ofEventArgs &e){
            ofSetWindowTitle(_applicationName +"["+_applicationVersion+"] @"+ofToString((int)(ofGetFrameRate()))+"fps");
        }*/
        
        /* MAIN GUI */
        void ofxGUIGenerate(int _defW = 200) {

            ofxAppGui.setDefaultTextPadding(6);
            ofxAppGui.setDefaultWidth(_defW);
            ofxAppGui.setDefaultHeight(14);
            //
            
            ofxAppGui.setup("MAIN PARAMETERS", settingsFile, 0, 0);
            ofxAppGui.add(gWindowRatio.setWithoutEventNotifications(true));
            //ofxAppGui.add(gEnableMouseAutoHide);
            ofxAppGui.add(btnSave.set("SAVE CURRENT SETTINGS"));
            ofxAppGui.add(gMinimizeAll.set("MINIMIZE ALL", true));
            
            ofxAppGui.setPosition(0,0);
        }
        
        void ofxGUILoadSettings() {
            ofxAppGui.loadFromFile(settingsFile);
            
            gWindowRatio.addListener(this, &ofxFilikaApp::setWindowRatio);
            btnSave.addListener(this, &ofxFilikaApp::saveSettings);
            gMinimizeAll.addListener(this, &ofxFilikaApp::minimizeMaximize);
        }
        
        void ofxMouseAutoHide(float idleDurationBeforeHiding) {
            this->idleDurationBeforeHiding = idleDurationBeforeHiding;
        }
        
        virtual ~ofxFilikaApp() {}
        

        /// wrapper used to handle ofxFilikaApp magic behind the scenes ...
        /// do not use directly!
        class RunnerApp :
        
#ifdef TARGET_OF_IOS
        public ofxiOSApp {
#else
            public ofBaseApp {
#endif
            
            public:
                
                
                RunnerApp(ofxFilikaApp *app) {this->app = app;}
                ~RunnerApp() {delete app;}
                
                // ofBaseApp callbacks
                void setup() {
                    // Instantiate a main gui with preffered settings
                    ofSetBackgroundColor(0);
                    isGuiShow = false;
                    wCenter = ofGetWidth() * 0.5;
                    hCenter = ofGetHeight() * 0.5;
                    
                    app->ofxMouseAutoHide(3);
                    ofSetWindowPosition(0, 0);
                    
                    app->setup();
                }
                
                void update()
                {
                    app->mouseX = mouseX;
                    app->mouseY = mouseY;
                    
                    if(ofGetWindowMode() != ofWindowMode::OF_FULLSCREEN)
                        ofSetWindowTitle(_applicationName +"["+_applicationVersion+"] @" + ofToString(ofGetFrameRate(),1) + " FPS");
                    
                    // Mouse Auto Hide
                    if(app->gEnableMouseAutoHide) {
                        float timeSinceLastMouse = ofGetElapsedTimef() - app->lastTimeMouseMoved;
                        //    printf("time since last mouse moved: %f\n", timeSinceLastMouse);
                        if(app->cursorShowing && timeSinceLastMouse > app->idleDurationBeforeHiding) {
                            app->cursorShowing = false;
                            ofHideCursor();
                        }
                        
                        if(!app->cursorShowing && timeSinceLastMouse < app->idleDurationBeforeHiding) {
                            app->cursorShowing = true;
                            ofShowCursor();
                        }
                    }
                    
                    app->update();
                }
                
                void draw() {
                    app->draw();
                    
                    if(isGuiShow)
                        app->ofxAppGui.draw();
                }
                void exit() { app->exit(); }
                void windowResized(int w, int h) {
                    
                    if(currentFrame != ofGetFrameNum()) {
                        wCenter = w * 0.5;
                        hCenter = h * 0.5;                        
                        app->windowResized(w, h);
                    }
                    
                    currentFrame = ofGetFrameNum();
                }
                void keyPressed(ofKeyEventArgs & key) {
                    
                    /*if(key.key == OF_KEY_TAB) {
                        isGuiShow = !isGuiShow;
                    }else if(key.key == ' ') {
                        // full screen
                        ofToggleFullscreen();
                    }*/
                    
                    // CMD + key
                    if(key.modifiers == 16) {
                        if(key.key == 'g') {
                            isGuiShow = !isGuiShow;
                            if(isGuiShow)
                                app->gEnableMouseAutoHide = false;
                            else
                                app->gEnableMouseAutoHide = true;
                        }
                        
                        if(key.key == 'f')
                            ofToggleFullscreen();
                    }
                    
                    
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    
                    // Send Event to main app at last
                    app->keyPressed(key);
                }
                void keyReleased(ofKeyEventArgs & key) {
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    app->keyReleased(key);
                }
                
                void mouseMoved(int x, int y) {
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    app->mouseMoved(x, y);
                }
                void mouseDragged(int x, int y, int button) {
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    app->mouseDragged(x, y, button);
                }
                void mousePressed(int x, int y, int button) {
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    app->mousePressed(x, y, button);
                }
                void mouseReleased(int x, int y, int button) {
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    app->mouseReleased(x, y, button);
                }
                void mouseScrolled(int x, int y, float scrollX, float scrollY) {
                    // Mouse auto hide
                    app->lastTimeMouseMoved = ofGetElapsedTimef();
                    app->mouseScrolled(x, y, scrollX, scrollY);
                }
                void mouseEntered(int x, int y) { app->mouseEntered(x, y); }
                void mouseExited(int x, int y) { app->mouseExited(x, y); }
                
                void dragEvent(ofDragInfo dragInfo) { app->dragEvent(dragInfo); }
                void gotMessage(ofMessage msg) { app->gotMessage(msg); }
                
                void touchDown(int x, int y, int id) { app->touchDown(x, y, id); }
                void touchMoved(int x, int y, int id) { app->touchMoved(x, y, id); }
                void touchUp(int x, int y, int id) { app->touchUp(x, y, id); }
                void touchDoubleTap(int x, int y, int id) { app->touchDoubleTap(x, y, id); }
                void touchCancelled(int x, int y, int id) { app->touchCancelled(x, y, id); }
                
#ifdef TARGET_OF_IOS
                // ofxIOS callbacks
                void lostFocus() { app->lostFocus(); }
                void gotFocus() { app->gotFocus(); }
                void gotMemoryWarning() { app->gotMemoryWarning(); }
                void deviceOrientationChanged(int newOrientation) { app->deviceOrientationChanged(newOrientation); }
#endif
                
                // ofBaseSoundInput callbacks
                void audioIn(ofSoundBuffer& buffer) { app->audioIn(buffer); }
                void audioIn(float *input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) { app->audioIn(input, bufferSize, nChannels, deviceID, tickCount); }
                void audioIn(float *input, int bufferSize, int nChannels ) { app->audioIn(input, bufferSize, nChannels); }
                void audioReceived(float *input, int bufferSize, int nChannels) { app->audioIn(input, bufferSize, nChannels); }
                
                // ofBaseSoundOutput callbacks
                void audioOut(ofSoundBuffer& buffer) { app->audioOut(buffer); }
                void audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) { app->audioOut(output, bufferSize, nChannels, deviceID, tickCount); }
                void audioOut(float *output, int bufferSize, int nChannels) { app->audioOut(output, bufferSize, nChannels); }
                void audioRequested(float *output, int bufferSize, int nChannels) { app->audioOut(output, bufferSize, nChannels); }
                
                ofxFilikaApp* getAppPtr() {return app;}
                
                
            private:
                // Global variables for included sub scene classes
                //int wCenter, hCenter;
                int currentFrame;
                ofxFilikaApp *app;
            public:
                int wCenter, hCenter;
                
                bool isGuiShow;
                
                std::string _applicationName;
                std::string _applicationVersion;
                std::string _organizationName;
                std::string _organizationDomain;
            };
            
            friend class RunnerApp; ///< used to wrap this app
        };
        
        // GLOBAL INLINE CONST VARS
        inline void ofxSetApplicationName(std::string applicationName){
            ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_applicationName = applicationName;
        }
        inline void ofxSetApplicationVersion(std::string applicationVersion){
            ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_applicationVersion = applicationVersion;
        }
        inline void ofxSetApplicationVersion(int major, int minor, int patch){
           ofxSetApplicationVersion(ofToString(major)+"."+ofToString(minor)+"."+ofToString(patch));
        }
        inline void ofxSetOrganizationName(std::string organizationName){
            ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_organizationName = organizationName;
        }
        inline void ofxSetOrganizationDomain(std::string organizationDomain){
            ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_organizationDomain = organizationDomain;
        }
        
        inline void ofxSetDebugMode(bool _isGuiShow) {
            ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->isGuiShow = _isGuiShow;
            //gEnableMouseAutoHide ^= _isGuiShow;
            if(_isGuiShow)
                ofLogToConsole();
        }
        
        inline std::string ofxGetApplicationName(){
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_applicationName;
        }
        inline std::string ofxGetApplicationVersion(){
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_applicationVersion;
        }
        inline std::string ofxGetOrganizationName(){
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_organizationName;
        }
        inline std::string ofxGetOrganizationDomain(){
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->_organizationDomain;
        }
        
        inline int ofxGetWinCenterX() {
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->wCenter;
        }
        
        inline int ofxGetWinCenterY() {
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->hCenter;
        }
        
        inline bool ofxGetDebugMode() {
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->isGuiShow;
        }
        
        inline int ofxGetGUIWidth() {
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->getAppPtr()->ofxAppGui.getWidth();
        }
        /// replace ofRunApp with this in main.cpp, make sure to call ofxGetAppPtr()
        /// instead of ofGetAppPtr() if you need access to the global ofxApp pointer
        inline void ofRunAppWithAppUtils(ofxFilikaApp *app) {
            
            // wrap up user app with runner
#ifdef TARGET_OF_IOS
            ofRunApp((ofxiOSApp *) new ofxFilikaApp::RunnerApp(app));
#else
            ofRunApp((ofBaseApp *) new ofxFilikaApp::RunnerApp(app));
            //ofRunApp(new ofxCppSketch("ofApp", __FILE__));
            
#endif
        }
        
        /// get the global ofxApp ptr, important! use this INSTEAD of ofGetAppPtr()
        /// if you started your app with ofRunAppWithAppUtils()
        inline ofxFilikaApp* ofxGetAppPtr() {
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->getAppPtr();
        }
        
#endif /* ofxFilikaApp_h */
