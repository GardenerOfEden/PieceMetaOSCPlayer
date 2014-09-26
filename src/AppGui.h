//
//  AppGui.h
//  PieceMetaOSCPlayer
//
//  Created by Anton on 02/09/14.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"
#include "ofxUI.h"

class AppGui {
    
public:
    void setupGUI();
    void createChannelMixer();
    
    ofxUICanvas* mainGui;
    ofxUICanvas* urlGui;
    ofxUIScrollableCanvas* channelGui;
};