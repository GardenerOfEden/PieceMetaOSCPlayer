//
//  AppGui.cpp
//  PieceMetaOSCPlayer
//
//  Created by Anton on 02/09/14.
//
//

#include "AppGui.h"

void AppGui::setupGUI()
{
    
    urlGui = new ofxUICanvas();
    urlGui->setPosition(0.0, 0.0);
    urlGui->setWidth(ofGetWindowWidth());
    urlGui->setHeight(100.0);
    urlGui->setColorBack(ofxUIColor(100,100,100,20));
    urlGui->setColorFill(ofxUIColor(60,60,60,255));
    urlGui->setColorFillHighlight(ofxUIColor(60,120,255,255));
    urlGui->setColorOutline(ofxUIColor(60,60,60,255));
    urlGui->setColorOutlineHighlight(ofxUIColor(60,120,255,255));
    
    urlGui->setName("DATA");
    urlGui->addLabel("DATA URL");
    urlGui->addSpacer();
    urlGui->addTextInput("DATA_URL", "")->setAutoClear(false);
    urlGui->addButton("PASTE DATA URL FROM CLIPBOARD", false);
    urlGui->addButton("LOAD DATA", false);
    
    //ofAddListener(urlGui->newGUIEvent, this, &ofApp::guiEvent);
    urlGui->loadSettings("url");
    
    mainGui = new ofxUICanvas();
    mainGui->setPosition(ofGetWindowWidth()-150.0, 100.0);
    mainGui->setWidth(150.0);
    mainGui->setHeight(ofGetWindowHeight()-100.0);
    mainGui->setColorBack(ofxUIColor(100,100,100,20));
    mainGui->setColorFill(ofxUIColor(60,60,60,255));
    mainGui->setColorFillHighlight(ofxUIColor(60,120,255,255));
    mainGui->setColorOutline(ofxUIColor(60,60,60,255));
    mainGui->setColorOutlineHighlight(ofxUIColor(60,120,255,255));
    
    mainGui->setName("SETTINGS");
    mainGui->addLabel("SETTINGS");
    
    mainGui->addSpacer();
    
    mainGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	mainGui->addToggle("PLAY", false, 20.0, 20.0, 0.0, 0.0);
    mainGui->addToggle("LOOP", false, 20.0, 20.0, 0.0, 0.0);
    mainGui->addButton("REWIND", false, 20.0, 20.0, 0.0, 0.0);
    
    mainGui->addSpacer();
    
    mainGui->setWidgetFontSize(OFX_UI_FONT_LARGE);
    
    mainGui->addLabel("TARGET FPS", OFX_UI_FONT_SMALL);
    mainGui->addTextInput("TARGET_FPS", "")->setAutoClear(false);
    ((ofxUITextInput *) mainGui->getWidget("TARGET_FPS"))->setTextString(ofToString(30));
    
    mainGui->addSpacer();
    
    mainGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    mainGui->addLabel("IP ADDRESS", OFX_UI_FONT_SMALL);
    //mainGui->addTextInput("IP_ADDRESS", OSC_HOST)->setAutoClear(false);
    
    mainGui->addLabel("UDP PORT", OFX_UI_FONT_SMALL);
    //mainGui->addTextInput("UDP_PORT", ofToString(OSC_PORT))->setAutoClear(false);
    
    //ofAddListener(mainGui->newGUIEvent, this, &ofApp::guiEvent);
    
	mainGui->loadSettings("settings");
	
}

void AppGui::createChannelMixer()
{
    if (channelGui) {
        delete channelGui;
    }
    channelGui = new ofxUIScrollableCanvas();
    channelGui->setPosition(0.0, 100.0);
    channelGui->setScrollableDirections(false, true);
    channelGui->setWidth(ofGetWindowWidth()-150.0);
    channelGui->setHeight(ofGetWindowHeight()-100.0);
    channelGui->setColorBack(ofxUIColor(100,100,100,20));
    channelGui->setColorFill(ofxUIColor(60,60,60,255));
    channelGui->setColorFillHighlight(ofxUIColor(60,120,255,255));
    channelGui->setColorOutline(ofxUIColor(60,60,60,255));
    channelGui->setColorOutlineHighlight(ofxUIColor(60,120,255,255));
    
    channelGui->setName("CHANNELS");
    
    channelGui->setWidgetFontSize(OFX_UI_FONT_LARGE);
    //channelGui->addLabel(dataPackage.title);
    
    channelGui->addSpacer();
    
    channelGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    /*
    unsigned int channelSize = dataPackage.dataChannels.size();
    for(unsigned int c = 0; c < channelSize; ++c)
    {
        unsigned int streamSize = dataPackage.dataChannels[c].dataStreams.size();
        for(unsigned int s = 0; s < streamSize; ++s)
        {
        std:string streamPath = dataPackage.dataChannels[c].title + "/" + dataPackage.dataChannels[c].dataStreams[s].group;
            channelGui->addToggle(streamPath, true, 20.0, 20.0, 0.0, 0.0);
            channelGui->addSpacer();
        }
    }
     */
    
    channelGui->autoSizeToFitWidgets();
    //ofAddListener(channelGui->newGUIEvent, this, &ofApp::guiEvent);
}