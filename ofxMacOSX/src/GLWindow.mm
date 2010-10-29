//
//  GLWindow.m
//  Touch & Try US OPENGL
//
//  Created by Mehmet Akten on 26/03/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

#import "ofMain.h"
#import "GLWindow.h"

@implementation GLWindow


-(id) initWithContentRect:(NSRect)windowRect {
	if(self = [super initWithContentRect:windowRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES]) {
		[self setOpaque:YES];
//		[self setLevel:NSMainMenuWindowLevel+1];
		[self setBackgroundColor:[NSColor blackColor]]; 
		[self setMovableByWindowBackground:YES]; 
		[self makeKeyAndOrderFront:self];
		[self setHasShadow:NO];
		[self setAcceptsMouseMovedEvents:YES];
		self.delegate = self;
	}
	return self;
}		

- (void)windowDidResize:(NSNotification *)notification {
	NSLog(@"windowDidResize");
	
	static ofResizeEventArgs resizeEventArgs;
	
	int w = self.frame.size.width;
	int h = self.frame.size.height;
	
	if(ofGetAppPtr()) ofGetAppPtr()->windowResized(w, h);
	
#ifdef OF_USING_POCO
	resizeEventArgs.width = w;
	resizeEventArgs.height = h;
	ofNotifyEvent( ofEvents.windowResized, resizeEventArgs );
#endif
	
	[self.contentView setNeedsDisplay:YES];
}

- (BOOL)canBecomeKeyWindow {
	return YES;
}


@end
