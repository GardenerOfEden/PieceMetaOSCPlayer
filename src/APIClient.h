//
//  APIClient.h
//  PieceMetaOSCPlayer
//
//  Created by Anton on 01/09/14.
//
//
#pragma once

#include <iostream>
#include <vector>
#include "ofxJSONElement.h"

struct data_stream_t
{
    string title;
    string group;
    string streamId;
    vector<std::map<std::string, float> > dataFrames;
};

struct data_channel_t
{
    string title;
    string channelId;
    vector<data_stream_t> dataStreams;
};

struct data_package_t
{
    string title;
    string packageId;
    float frameRate;
    unsigned int frameCount;
    vector<data_channel_t> dataChannels;
};

class APIClient {

public:
    bool complete;
    data_package_t loadUrl(std::string url);
};
