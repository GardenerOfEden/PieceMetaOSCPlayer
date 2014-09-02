#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPopStyle();
    
    ofSetWindowTitle("PieceMetaOSCPlayer");
    
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
    
    ofAddListener(urlGui->newGUIEvent, this, &ofApp::guiEvent);
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
    mainGui->addTextInput("IP_ADDRESS", OSC_HOST)->setAutoClear(false);
    
    mainGui->addLabel("UDP PORT", OFX_UI_FONT_SMALL);
    mainGui->addTextInput("UDP_PORT", ofToString(OSC_PORT))->setAutoClear(false);
    
    ofAddListener(mainGui->newGUIEvent, this, &ofApp::guiEvent);
    
	mainGui->loadSettings("settings");
	
}

void ofApp::createChannelMixer()
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
        std::string port = ((ofxUITextInput *) mainGui->getWidget("UDP_PORT"))->getTextString();
        ofApp::setupOSC(ti->getTextString(), ofToInt(port));
    }
    else if (e.getName() == "UDP_PORT")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        std::string host = ((ofxUITextInput *) mainGui->getWidget("IP_ADDRESS"))->getTextString();
        ofApp::setupOSC(host, ofToInt(ti->getTextString()));
    }
    else if (e.getName() == "PASTE DATA URL FROM CLIPBOARD")
    {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        ((ofxUITextInput *) urlGui->getWidget("DATA_URL"))->setTextString(ClipboardAccess::getTextFromClipboard());
    }
    else if (e.getName() == "LOAD DATA")
    {
        ((ofxUIToggle *) mainGui->getWidget("PLAY"))->setValue(false);
        active = false;
        currentFrame = 0;
        std::string packageUrl = ((ofxUITextInput *) urlGui->getWidget("DATA_URL"))->getTextString();
        dataPackage = apiClient.loadUrl(packageUrl);
        if (apiClient.complete)
        {
            ((ofxUITextInput *) mainGui->getWidget("TARGET_FPS"))->setTextString(ofToString(dataPackage.frameRate));
            ofSetFrameRate(dataPackage.frameRate);
        }
        else
        {
            ((ofxUITextInput *) mainGui->getWidget("TARGET_FPS"))->setTextString(ofToString(30));
            ofSetFrameRate(30);
        }
        ofApp::createChannelMixer();
    }
}

