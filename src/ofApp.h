#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "APIClient.h"
#include "ClipboardAccess.h"

#define OSC_HOST "127.0.0.1"
#define OSC_PORT 8888

class ofApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();
        void gotMessage(ofMessage msg);
    
        void exit();
        void windowResized(int w, int h);
        void guiEvent(ofxUIEventArgs &e);
        void channelGuiEvent(ofxUIEventArgs &e);
        void setupGUI();
        void createChannelMixer();

        void setupOSC(std::string host, int port);
        void updateJSON(std::string url);
    
        int currentFrame;
        bool active;
        bool loop;
    
        vector<std::string> activeTracks;
    
        ofxOscSender sender;
        data_package_t dataPackage;
    
        ofxUICanvas* mainGui;
        ofxUICanvas* urlGui;
        ofxUIScrollableCanvas* channelGui;
    
        APIClient apiClient;
		
};
