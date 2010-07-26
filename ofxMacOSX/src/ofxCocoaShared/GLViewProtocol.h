//
//  GLViewProtocol.h
//  Cocoa Test
//
//  Created by Mehmet Akten on 17/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

#define OF_CURRENT_SCREEN		-1
#define OF_ALL_SCREENS			-2

#import "CocoaDefines.h"

@protocol GLViewProtocol

-(id)window;
-(OFViewRect)frame;
-(OFViewRect)bounds;

-(void)setCurrentContext;
-(void)flush;

- (void)goFullscreen:(int)whichScreen;
-(void)goFullscreen;	// defaults to fulscreen only on current screen
- (void)goWindow;


@end

