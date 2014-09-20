//
//  ClipboardAccess.h
//  PieceMetaOSCPlayer
//
//  Created by Anton on 02/09/14.
//
//

#pragma once

#ifdef __MINGW32__
#include "windows.h"
#endif

#include <iostream>
#include "ofMain.h"

class ClipboardAccess {
    
public:
    static char* getTextFromClipboard();
};
