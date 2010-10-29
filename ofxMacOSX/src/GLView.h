

#import "GLee.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

#import "ofAppCocoaWindow.h"

#define OF_CURRENT_SCREEN		-1
#define OF_ALL_SCREENS			-2

#import "CocoaDefines.h"

@interface GLView : NSOpenGLView {
	NSRect savedWindowFrame;
	
	NSOpenGLContext *openGLContext;
	NSOpenGLPixelFormat *pixelFormat;
	
	float			targetFrameRate;
	BOOL			useDisplayLink;
	CVDisplayLinkRef displayLink;
	NSTimer			*timer;
	BOOL			isAnimating;
}

-(id) initWithFrame:(NSRect)frameRect;
-(id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context;

-(NSOpenGLContext*) openGLContext;


-(void) updateAndDraw;
-(void) startAnimation;
-(void) stopAnimation;
-(void) toggleAnimation;

-(void) setFrameRate:(float)rate;

//-------



//-(void)setCurrentContext;
//-(void)flush;

-(void)goFullscreen:(int)whichScreen;
-(void)goFullscreen;	// defaults to fulscreen only on current screen
-(void)goWindow;


@end
