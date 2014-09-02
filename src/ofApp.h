#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "APIClient.h"

#define HOST "127.0.0.1"
#define PORT 8888

class ofApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();
        void gotMessage(ofMessage msg);
    
        void exit();
        void windowResized(int w, int h);
        void guiEvent(ofxUIEventArgs &e);
        void setupGUI();
        void createChannelMixer();

        void setupOSC(std::string host, int port);
        void updateJSON(std::string url);
    
        int currentFrame;
        bool active;
        bool loop;
    
        ofxOscSender sender;
        data_package_t dataPackage;
    
        ofxUICanvas* mainGui;
        ofxUICanvas* urlGui;
        ofxUIScrollableCanvas* channelGui;
    
        APIClient apiClient;
		
};
