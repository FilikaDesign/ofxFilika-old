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
        
    public:
        
        ofxFilikaApp() {}
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
                    app->setup();
                }
                
                void update()
                {
                    app->mouseX = mouseX;
                    app->mouseY = mouseY;
                    
                    app->update();
                }
                
                void draw() { app->draw(); }
                void exit() { app->exit(); }
                void windowResized(int w, int h) { app->windowResized(w, h); }
                void keyPressed(ofKeyEventArgs & key) { app->keyPressed(key); }
                void keyReleased(ofKeyEventArgs & key) { app->keyReleased(key); }
                
                void mouseMoved(int x, int y) { app->mouseMoved(x, y); }
                void mouseDragged(int x, int y, int button) { app->mouseDragged(x, y, button); }
                void mousePressed(int x, int y, int button) { app->mousePressed(x, y, button); }
                void mouseReleased(int x, int y, int button) { app->mouseReleased(x, y, button); }
                void mouseScrolled(int x, int y, float scrollX, float scrollY) { app->mouseScrolled(x, y, scrollX, scrollY); }
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
                
                ofxFilikaApp *app;
            };
            
            friend class RunnerApp; ///< used to wrap this app
        };

        
        
        
        /// replace ofRunApp with this in main.cpp, make sure to call ofxGetAppPtr()
        /// instead of ofGetAppPtr() if you need access to the global ofxApp pointer
        inline void ofRunAppWithAppUtils(ofxFilikaApp *app) {
            
            // wrap up user app with runner
#ifdef TARGET_OF_IOS
            ofRunApp((ofxiOSApp *) new ofxFilikaApp::RunnerApp(app));
#else
            ofRunApp((ofBaseApp *) new ofxFilikaApp::RunnerApp(app));
#endif
        }
        
        /// get the global ofxApp ptr, important! use this INSTEAD of ofGetAppPtr()
        /// if you started your app with ofRunAppWithAppUtils()
        inline ofxFilikaApp* ofxGetAppPtr() {
            return ((ofxFilikaApp::RunnerApp *) ofGetAppPtr())->getAppPtr();
        }
        
#endif /* ofxFilikaApp_h */
