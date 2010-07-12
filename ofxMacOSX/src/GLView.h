

#import "GLee.h"
#import <Cocoa/Cocoa.h>

#import "GLViewProtocol.h"

class ofAppCocoaWindow;

@interface GLView : NSOpenGLView <GLViewProtocol> {
	ofAppCocoaWindow *cocoaWindowPtr;
}

//-(void)drawRect:(NSRect)bounds;

- (void) keyDown:(NSEvent *)theEvent;
- (void) mouseDown:(NSEvent *)theEvent;
- (void) rightMouseDown:(NSEvent *)theEvent;
- (void) otherMouseDown:(NSEvent *)theEvent;
- (void) mouseUp:(NSEvent *)theEvent;
- (void) rightMouseUp:(NSEvent *)theEvent;
- (void) otherMouseUp:(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;
- (void) scrollWheel:(NSEvent *)theEvent;
- (void) rightMouseDragged:(NSEvent *)theEvent;
- (void) otherMouseDragged:(NSEvent *)theEvent;
- (void) mouseMoved:(NSEvent *)theEvent;


// GLViewProtocol:
-(void)setCurrentContext;
-(void)flush;

- (void)goFullscreen:(int)whichScreen;
-(void)goFullscreen;	// defaults to fulscreen only on current screen
- (void)goWindow;


@end
