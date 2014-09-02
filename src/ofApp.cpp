#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPopStyle();
    
    ofSetWindowTitle("PieceMetaOSCPlayer");
    
    ofApp::setupOSC(HOST, PORT);
    ofApp::setupGUI();
    
    currentFrame = 0;
}

void ofApp::setupOSC(std::string host, int port)
{
    sender.setup(host, port);
}


void ofApp::update()
{
    
}


void ofApp::draw()
{
    ofBackground(0xFFFFFF);
    ofSetHexColor(0x000000);
    
    if (active && apiClient.complete) {
        ofxOscBundle dataBundle;
        unsigned int channelSize = dataPackage.dataChannels.size();
        
        for(unsigned int c = 0; c < channelSize; ++c)
        {
            unsigned int streamSize = dataPackage.dataChannels[c].dataStreams.size();
            for(unsigned int s = 0; s < streamSize; ++s)
            {
                
                if (currentFrame < dataPackage.dataChannels[c].dataStreams[s].dataFrames.size())
                {
                    ofxOscMessage m;
                    m.setAddress("/" + dataPackage.dataChannels[c].title + "/" + dataPackage.dataChannels[c].dataStreams[s].group);
                    
                    if (dataPackage.dataChannels[c].dataStreams[s].group == "")
                    {
                        m.setAddress("/" + dataPackage.dataChannels[c].title + "/" + dataPackage.dataChannels[c].dataStreams[s].group + "/" + dataPackage.dataChannels[c].dataStreams[s].title);
                    }
                    
                    for (std::map<std::string, float>::iterator i = dataPackage.dataChannels[c].dataStreams[s].dataFrames[currentFrame].begin(); i != dataPackage.dataChannels[c].dataStreams[s].dataFrames[currentFrame].end(); i++)
                    {
                        m.addFloatArg(i->second);
                    }
                    dataBundle.addMessage(m);
                }
            }
        }
        
        sender.sendBundle(dataBundle);
        
        currentFrame += 1;
        if (currentFrame == dataPackage.frameCount) {
            currentFrame = 0;
            if (!loop)
            {
                ((ofxUIToggle *) mainGui->getWidget("PLAY"))->setValue(false);
                active = false;
            }
        }
    }
    
    char fps[5];
    sprintf(fps, "%.2f", ofGetFrameRate());
    ofDrawBitmapString(ofToString(fps) + "fps", ofGetWindowWidth()-140.0, ofGetWindowHeight()-20.0);
	ofDrawBitmapString("Frame " + ofToString(currentFrame+1), ofGetWindowWidth()-140.0, ofGetWindowHeight()-40.0);
}

void ofApp::gotMessage(ofMessage msg)
{
    ofLogNotice("ofApp::gotMessage") << msg.message << endl;
}

// ofxUI

void ofApp::setupGUI()
{
    
    urlGui = new ofxUICanvas();
    urlGui->setPosition(0.0, 0.0);
    urlGui->setWidth(ofGetWindowWidth());
    urlGui->setHeight(80.0);
    urlGui->setColorBack(ofxUIColor(100,100,100,20));
    urlGui->setColorFill(ofxUIColor(60,60,60,255));
    urlGui->setColorFillHighlight(ofxUIColor(60,120,255,255));
    urlGui->setColorOutline(ofxUIColor(60,60,60,255));
    urlGui->setColorOutlineHighlight(ofxUIColor(60,120,255,255));
    urlGui->setAutoDraw(true);
    urlGui->setAutoUpdate(true);
    
    urlGui->setName("DATA");
    urlGui->addLabel("DATA PACKAGE ID");
    urlGui->addSpacer();
    urlGui->addTextInput("DATA_URL", "5404cff005e267d804878526")->setAutoClear(false);
    urlGui->addButton("LOAD_DATA", true);
    
    ofAddListener(urlGui->newGUIEvent, this, &ofApp::guiEvent);
    urlGui->loadSettings("url");
    
    mainGui = new ofxUICanvas();
    mainGui->setPosition(ofGetWindowWidth()-150.0, 80.0);
    mainGui->setWidth(150.0);
    mainGui->setHeight(ofGetWindowHeight()-80.0);
    mainGui->setColorBack(ofxUIColor(100,100,100,20));
    mainGui->setColorFill(ofxUIColor(60,60,60,255));
    mainGui->setColorFillHighlight(ofxUIColor(60,120,255,255));
    mainGui->setColorOutline(ofxUIColor(60,60,60,255));
    mainGui->setColorOutlineHighlight(ofxUIColor(60,120,255,255));
    mainGui->setAutoDraw(true);
    mainGui->setAutoUpdate(true);
    
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
    mainGui->addTextInput("IP_ADDRESS", "127.0.0.1")->setAutoClear(false);
    
    mainGui->addLabel("UDP PORT", OFX_UI_FONT_SMALL);
    mainGui->addTextInput("UDP_PORT", "8888")->setAutoClear(false);
    
    ofAddListener(mainGui->newGUIEvent, this, &ofApp::guiEvent);
    
	mainGui->loadSettings("settings");
	
}

void ofApp::createChannelMixer()
{
    if (channelGui) {
        delete channelGui;
    }
    channelGui = new ofxUIScrollableCanvas();
    channelGui->setPosition(0.0, 80.0);
    channelGui->setScrollableDirections(false, true);
    channelGui->setWidth(ofGetWindowWidth()-150.0);
    channelGui->setHeight(ofGetWindowHeight()-80.0);
    channelGui->setColorBack(ofxUIColor(100,100,100,20));
    channelGui->setColorFill(ofxUIColor(60,60,60,255));
    channelGui->setColorFillHighlight(ofxUIColor(60,120,255,255));
    channelGui->setColorOutline(ofxUIColor(60,60,60,255));
    channelGui->setColorOutlineHighlight(ofxUIColor(60,120,255,255));
    channelGui->setAutoDraw(true);
    channelGui->setAutoUpdate(true);
    
    channelGui->setName("CHANNELS");
    
    channelGui->setWidgetFontSize(OFX_UI_FONT_LARGE);
    channelGui->addLabel(dataPackage.title);
    
    channelGui->addSpacer();
    
    channelGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
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
    
    channelGui->autoSizeToFitWidgets();
    ofAddListener(channelGui->newGUIEvent, this, &ofApp::guiEvent);
}

void ofApp::exit()
{
	mainGui->saveSettings("settings");
    urlGui->saveSettings("url");
    delete mainGui;
    delete urlGui;
    delete channelGui;
}

void ofApp::windowResized(int w, int h)
{
    if (channelGui) {
        channelGui->setWidth(ofGetWindowWidth()-150.0);
    }
    urlGui->setWidth(ofGetWindowWidth());
    mainGui->getRect()->setX(ofGetWindowWidth()-150.0);
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
	if (e.getName() == "PLAY")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        active = toggle->getValue();
    }
    else if (e.getName() == "LOOP")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        loop = toggle->getValue();
    }
    else if (e.getName() == "REWIND")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ((ofxUIToggle *) mainGui->getWidget("PLAY"))->setValue(false);
        active = false;
        currentFrame = 0;
    }
    else if (e.getName() == "TARGET_FPS")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        string output = ti->getTextString();
        float fps = ofToFloat(output);
        if (fps>.0) {
            ofSetFrameRate(fps);
        }
    }
    else if (e.getName() == "IP_ADDRESS")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        ofApp::setupOSC(ti->getTextString(), PORT);
    }
    else if (e.getName() == "UDP_PORT")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
    }
    else if (e.getName() == "LOAD_DATA")
    {
        ((ofxUIToggle *) mainGui->getWidget("PLAY"))->setValue(false);
        active = false;
        currentFrame = 0;
        std::string packageId = ((ofxUITextInput *) urlGui->getWidget("DATA_URL"))->getTextString();
        dataPackage = apiClient.find(packageId);
        ((ofxUITextInput *) mainGui->getWidget("TARGET_FPS"))->setTextString(ofToString(dataPackage.frameRate));
        ofSetFrameRate(dataPackage.frameRate);
        ofApp::createChannelMixer();
    }
}

