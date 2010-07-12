

#import "GLee.h"
#include "ofMain.h"
#import "ofxAppDelegate.h"
#import "ofAppCocoaWindow.h"

@implementation ofxAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	NSLog(@"ofxAppDelegate::applicationDidFinishLaunching");
	
	ofSetDataPathRoot("data/");
	[super applicationDidFinishLaunching];
}


-(IBAction)toggleFullscreen:(id)sender {
	ofToggleFullscreen();
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
	return YES;
}

-(IBAction)goFullScreen:(id)sender {
	[ofxGetCocoaWindow()->glView goFullscreen:((NSControl*)sender).tag];
	
}



@end
