//
//  ClipboardAccess.cpp
//  PieceMetaOSCPlayer
//
//  Created by Anton on 02/09/14.
//
//

#include "ClipboardAccess.h"

char* ClipboardAccess::getTextFromClipboard()
{
    char* data;

    #ifdef __APPLE__

	OSStatus             err = noErr;
    ItemCount            itemCount;
	PasteboardSyncFlags  syncFlags;
	static PasteboardRef inPasteboard = NULL;
	PasteboardCreate( kPasteboardClipboard, &inPasteboard );
	data = "";

	syncFlags = PasteboardSynchronize( inPasteboard );
	err = badPasteboardSyncErr;

    err = PasteboardGetItemCount( inPasteboard, &itemCount );
    require_noerr( err, CantGetPasteboardItemCount );

    for( int itemIndex = 1; itemIndex <= itemCount; itemIndex++ ) {
        PasteboardItemID  itemID;
		CFDataRef  flavorData;

        err = PasteboardGetItemIdentifier( inPasteboard, itemIndex, &itemID );
        require_noerr( err, CantGetPasteboardItemIdentifier );

		err = PasteboardCopyItemFlavorData( inPasteboard, itemID, CFSTR("public.utf8-plain-text"), &flavorData );
		data = (char*)CFDataGetBytePtr(flavorData);

    CantGetPasteboardItemIdentifier:
		;
    }

CantGetPasteboardItemCount:

    #endif

    #ifdef __MINGW32__
    std::string clipBoardText = "";
    if (OpenClipboard(NULL)){
        HANDLE hClipboardData = GetClipboardData(CF_TEXT);
        if(IsClipboardFormatAvailable(CF_TEXT)){
            char *pchData = (char*)GlobalLock(hClipboardData);
            clipBoardText.assign(pchData);
            GlobalUnlock(hClipboardData);
            CloseClipboard();
        }
    }
    data = strstr(clipBoardText.c_str(), "");
    #endif

    #ifdef __linux__
    #endif

    return data;
}
