
#import "GLView.h"
#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/glext.h>
#import <OpenGL/glu.h>

#import "ofAppCocoaWindow.h"




@implementation GLView

- (id)initWithFrame:(NSRect)frame {
	NSLog(@"GLView::initWithFrame %@", NSStringFromRect(frame));
	
	cocoaWindowPtr = (ofAppCocoaWindow*)ofxGetCocoaWindow();
	
	NSOpenGLPixelFormat *fmt = nil;
	
	/* Choose a pixel format */
	if(cocoaWindowPtr->numberOfFSAASamples) {
		NSOpenGLPixelFormatAttribute attribs[] = {
			NSOpenGLPFAAccelerated,
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFADepthSize, 24,
			NSOpenGLPFAAlphaSize, 8,
			NSOpenGLPFAColorSize, 32,
			NSOpenGLPFASampleBuffers, 1,
			NSOpenGLPFASamples, cocoaWindowPtr->numberOfFSAASamples,
			NSOpenGLPFANoRecovery,
		0};
		
		NSLog(@"   trying Multisampling");
		fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
		if(fmt) {
			NSLog(@"      Multisampling supported");
			glEnable(GL_MULTISAMPLE);
		} else {
			NSLog(@"      Multisampling not supported");
		}
	}
	
	
	if(fmt == nil) {
		NSLog(@"   trying non multisampling");
		NSOpenGLPixelFormatAttribute attribs[] = {
			NSOpenGLPFAAccelerated,
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFADepthSize, 24,
			NSOpenGLPFAAlphaSize, 8,
			NSOpenGLPFAColorSize, 32,
			NSOpenGLPFANoRecovery,
		0};		
		
		fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
		glDisable(GL_MULTISAMPLE);
		if(fmt == nil) {
			NSLog(@"      not even that. fail");
		}
	} 
	
	self = [super initWithFrame:frame pixelFormat:fmt];
	NSLog(@"   window:%@", [self window]);
	[fmt release];
	
	GLint p	= 0;	
	[[self openGLContext] setValues:&p forParameter: NSOpenGLCPSurfaceOrder];
	
	p = 1;
	[[self openGLContext] setValues:&p forParameter: NSOpenGLCPSwapInterval];
	
	
	//	[[self openGLContext] setValues:&p forParameter: NSOpenGLCPSurfaceOpacity];
	
	return self;
}


- (void) awakeFromNib {
	NSLog(@"GLView::awakeFromNib, window:%@",[self window]);
	[[self window] setAcceptsMouseMovedEvents:YES]; 
	ofxGetCocoaWindow()->glView = self;
	ofxGetCocoaWindow()->initWindowSize();
}


//-(void)drawRect:(NSRect)bounds {
//}


- (BOOL)acceptsFirstResponder {
	return YES;
}

- (BOOL)becomeFirstResponder {
	return  YES;
}

- (BOOL)resignFirstResponder {
	return YES;
}

#pragma mark GLViewProtocol


- (void)goFullscreen:(int)whichScreen {
	if(whichScreen == OF_ALL_SCREENS) {
		// need to create window from scratch, not nib:
		//		NSScreen *screen = [[self window] screen];
		//		NSRect rect	= [screen frame];
		NSRect rect = NSZeroRect;
		for(NSScreen *s in [NSScreen screens]) {
			rect = NSUnionRect(rect, s.frame);
		}
		
		NSLog(@"%@", NSStringFromRect(rect));
		[[self window] setFrame:rect display:YES animate:YES];
		[[self window] setLevel:NSScreenSaverWindowLevel];
		[self setBounds:rect];
		SetSystemUIMode(kUIModeAllHidden,NULL);
	} else {
		NSScreen *screen;
		if(whichScreen == OF_CURRENT_SCREEN) screen = [[self window] screen];
		else screen = [[NSScreen screens] objectAtIndex:whichScreen];
		if([self respondsToSelector:@selector(isInFullScreenMode)]){
			[self enterFullScreenMode:screen
						  withOptions:[NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool: NO], NSFullScreenModeAllScreens, [NSNumber numberWithInt:NSNormalWindowLevel] ,NSFullScreenModeWindowLevel, nil]];
		}
	}
	
	ofxGetCocoaWindow()->windowMode = OF_FULLSCREEN;
}

-(void)goFullscreen {
	[self goFullscreen:OF_CURRENT_SCREEN];
}



// ---------------------------------
- (void) goWindow{
	if([self respondsToSelector:@selector(isInFullScreenMode)]){
		if([self isInFullScreenMode]){
			[self exitFullScreenModeWithOptions:nil];
		}
	}
	
	ofxGetCocoaWindow()->windowMode = OF_WINDOW;
}


-(void)setCurrentContext {
	[[self openGLContext] makeCurrentContext];
}

-(void)flush {
	[[self openGLContext] flushBuffer];
}


#pragma mark Events

-(void)keyDown:(NSEvent *)theEvent {
	//	NSLog(@"keyDown");
	NSString *characters = [theEvent characters];
	if ([characters length]) {
		unichar character = [characters characterAtIndex:0];
		cocoaWindowPtr->keyPressed(character);
	}
}

// ---------------------------------

- (void)mouseDown:(NSEvent *)theEvent {
	//	NSLog(@"mouseDown");
	if ([theEvent modifierFlags] & NSControlKeyMask) 
		[self rightMouseDown:theEvent];
	else if ([theEvent modifierFlags] & NSAlternateKeyMask) 
		[self otherMouseDown:theEvent];
	else {
		NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
		cocoaWindowPtr->mousePressed(location.x, location.y, 0);
	}
}

// ---------------------------------

- (void)rightMouseDown:(NSEvent *)theEvent {
	//	NSLog(@"rightMouseDown");
	
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	cocoaWindowPtr->mousePressed(location.x, location.y, 1);
	
}

// ---------------------------------

- (void)otherMouseDown:(NSEvent *)theEvent {
	//	NSLog(@"rightMouseDown");
	
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];	
	cocoaWindowPtr->mousePressed(location.x, location.y, 2);
}

- (void)mouseMoved:(NSEvent *)theEvent{
	//	NSLog(@"mouseMoved");
	
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];	
	cocoaWindowPtr->mouseMoved(location.x, location.y);
}


// ---------------------------------

- (void)mouseUp:(NSEvent *)theEvent {
	//	NSLog(@"mouseUp");
	cocoaWindowPtr->mouseReleased(0);
}

// ---------------------------------

- (void)rightMouseUp:(NSEvent *)theEvent {
	//	NSLog(@"rightMouseUp");
	cocoaWindowPtr->mouseReleased(1);
}

// ---------------------------------

- (void)otherMouseUp:(NSEvent *)theEvent {
	//	NSLog(@"otherMouseUp");
	cocoaWindowPtr->mouseReleased(2);
}

// ---------------------------------

- (void)mouseDragged:(NSEvent *)theEvent {
	//	NSLog(@"mouseDragged");
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	cocoaWindowPtr->mouseDragged(location.x, location.y, 0);
}

// ---------------------------------

- (void)rightMouseDragged:(NSEvent *)theEvent {
	//	NSLog(@"rightMouseDragged");
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	cocoaWindowPtr->mouseDragged(location.x, location.y, 1);
}

// ---------------------------------

- (void)otherMouseDragged:(NSEvent *)theEvent {
	//	NSLog(@"otherMouseDragged");
	
	NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	cocoaWindowPtr->mouseDragged(location.x, location.y, 2);
	
}

- (void)scrollWheel:(NSEvent *)theEvent {
	//	NSLog(@"scrollWheel");
	
	//	float wheelDelta = [theEvent deltaX] +[theEvent deltaY] + [theEvent deltaZ];
	//	if (wheelDelta)
	//	{
	//		GLfloat deltaAperture = wheelDelta * -camera.aperture / 200.0f;
	//
	//	}
}


@end
