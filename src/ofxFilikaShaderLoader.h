//
//  ofxFilikaShaderLoader.h
//  rawAV2
//
//  Created by alp tugan on 22.04.2020.
//

#ifndef ofxFilikaShaderLoader_h
#define ofxFilikaShaderLoader_h

#include "ofMain.h"


class ofxFilikaShaderLoader {
private:
    
    
    
    int sw, sh;
    ofFbo fbo;
    
    
    ofDirectory dir;
public:
    vector<ofShader> shaders;
    int num;
    ofxFilikaShaderLoader() { }
    
    void setSize(int w, int h) {
        sw = w;
        sh = h;
    }
    
    void setup(int w, int h) {
        sw = w;
        sh = h;
        
        num = 0;
        fbo.allocate(sw, sh);
    }
    
    void loadFromFile(string _name) {
        ofShader sh;
        sh.load( _name );
        
        shaders.push_back(sh);
    }
    
    void loadFromFolder(string _dir) {
        
        dir.listDir(_dir);
        //dir.allowExt(".vert");
        dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

        
        
        for (int i = 0; i < dir.getFiles().size(); i++){
            /*ostringstream digit;
            digit << setfill('0') << right << setw(2) << i;*/
            
            
            ofShader sh;
            sh.load( dir.getPath(i) );
            
            
            shaders.push_back(sh);
        }
        
    }
    
    /*void draw(int x = 0, int y = 0) {
     
         if (ofGetFrameNum() % int(ofRandom(1, 6)) == 0) {
         num = int(ofRandom(shaders.size()));
         }
     
        
        fbo.begin();
        shaders[num].begin();
        shaders[num].setUniform1f("time", ofGetElapsedTimef());
        shaders[num].setUniform2f("resolution", sw / 2, sh / 2);
        ofDrawRectangle(0, 0, sw, sh);
        shaders[num].end();
        fbo.end();
        
        fbo.draw(x, y);
    }
    
    void begin() {
        fbo.begin();
        shaders[num].begin();
    }
    
    
    void end() {
        
    }*/
};

#endif /* ofxFilikaShaderLoader_h */
