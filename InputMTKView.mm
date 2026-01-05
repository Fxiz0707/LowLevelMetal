#import "InputMTKView.h"
#import <Metal/Metal.h>

@implementation InputMTKView

- (instancetype)initWithFrame:(CGRect)frameRect device:(id<MTLDevice>)device
{
    self = [super initWithFrame:frameRect device:device];
    if (self) {
        [self setWantsLayer:YES];
        
        // Create tracking area to receive mouse moved events
        NSTrackingAreaOptions options = NSTrackingMouseMoved | 
                                        NSTrackingActiveAlways | 
                                        NSTrackingInVisibleRect |
                                        NSTrackingMouseEnteredAndExited;
        NSTrackingArea* trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds
                                                                    options:options
                                                                      owner:self
                                                                   userInfo:nil];
        [self addTrackingArea:trackingArea];
    }
    return self;
}

- (void)updateTrackingAreas
{
    [super updateTrackingAreas];
    // Tracking areas are automatically updated due to NSTrackingInVisibleRect
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)becomeFirstResponder
{
    return YES;
}

- (void)viewDidMoveToWindow
{
    [super viewDidMoveToWindow];
    // Enable mouse moved events on the window
    if (self.window) {
        [self.window setAcceptsMouseMovedEvents:YES];
        [self.window makeFirstResponder:self];
    }
}

#pragma mark - Keyboard Events

- (void)keyDown:(NSEvent *)event
{
    // Ignore key repeat events (only fire once when key is first pressed)
    if ([event isARepeat]) {
        return;
    }
    
    if (_inputHandler) {
        _inputHandler->keyDown([event keyCode]);
    }
}

- (void)keyUp:(NSEvent *)event
{
    if (_inputHandler) {
        _inputHandler->keyUp([event keyCode]);
    }
}

- (void)flagsChanged:(NSEvent *)event
{
    // Handle modifier keys (Shift, Control, etc.)
    if (_inputHandler) {
        unsigned short keyCode = [event keyCode];
        NSEventModifierFlags flags = [event modifierFlags];
        
        // Check if modifier is pressed or released based on flag state
        bool isPressed = false;
        switch (keyCode) {
            case 0x38: // Left Shift
            case 0x3C: // Right Shift
                isPressed = (flags & NSEventModifierFlagShift) != 0;
                break;
            case 0x3B: // Left Control
            case 0x3E: // Right Control
                isPressed = (flags & NSEventModifierFlagControl) != 0;
                break;
            case 0x3A: // Left Option/Alt
            case 0x3D: // Right Option/Alt
                isPressed = (flags & NSEventModifierFlagOption) != 0;
                break;
            case 0x37: // Left Command
            case 0x36: // Right Command
                isPressed = (flags & NSEventModifierFlagCommand) != 0;
                break;
        }
        
        if (isPressed) {
            _inputHandler->keyDown(keyCode);
        } else {
            _inputHandler->keyUp(keyCode);
        }
    }
}

#pragma mark - Mouse Events

- (void)mouseDown:(NSEvent *)event
{
    if (_inputHandler) {
        NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
        _inputHandler->setMousePosition(loc.x, loc.y);
        _inputHandler->mouseDown(0); // Left button
    }
}

- (void)mouseUp:(NSEvent *)event
{
    if (_inputHandler) {
        _inputHandler->mouseUp(0);
    }
}

- (void)rightMouseDown:(NSEvent *)event
{
    if (_inputHandler) {
        NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
        _inputHandler->setMousePosition(loc.x, loc.y);
        _inputHandler->mouseDown(1); // Right button
    }
}

- (void)rightMouseUp:(NSEvent *)event
{
    if (_inputHandler) {
        _inputHandler->mouseUp(1);
    }
}

- (void)otherMouseDown:(NSEvent *)event
{
    if (_inputHandler) {
        // buttonNumber: 0=left, 1=right, 2=middle, 3+=extra buttons
        NSInteger btn = [event buttonNumber];
        if (btn == 2) {
            _inputHandler->mouseDown(2); // Middle button
        }
        // Log for debugging
        printf("[DEBUG] otherMouseDown buttonNumber=%ld\n", (long)btn);
    }
}

- (void)otherMouseUp:(NSEvent *)event
{
    if (_inputHandler) {
        NSInteger btn = [event buttonNumber];
        if (btn == 2) {
            _inputHandler->mouseUp(2);
        }
    }
}

- (void)otherMouseDragged:(NSEvent *)event
{
    if (_inputHandler) {
        NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
        _inputHandler->setMousePosition(loc.x, loc.y);
        _inputHandler->setMouseDelta([event deltaX], [event deltaY]);
    }
}

- (void)mouseMoved:(NSEvent *)event
{
    if (_inputHandler) {
        NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
        _inputHandler->setMousePosition(loc.x, loc.y);
        _inputHandler->setMouseDelta([event deltaX], [event deltaY]);
    }
}

- (void)mouseDragged:(NSEvent *)event
{
    if (_inputHandler) {
        NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
        _inputHandler->setMousePosition(loc.x, loc.y);
        _inputHandler->setMouseDelta([event deltaX], [event deltaY]);
    }
}

- (void)rightMouseDragged:(NSEvent *)event
{
    if (_inputHandler) {
        NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
        _inputHandler->setMousePosition(loc.x, loc.y);
        _inputHandler->setMouseDelta([event deltaX], [event deltaY]);
    }
}

- (void)scrollWheel:(NSEvent *)event
{
    if (_inputHandler) {
        _inputHandler->setScrollDelta([event scrollingDeltaX], [event scrollingDeltaY]);
    }
}

@end

#pragma mark - C++ Interface

void* CreateInputMTKView(CGRect frame, void* device, InputHandler* inputHandler)
{
    InputMTKView* view = [[InputMTKView alloc] initWithFrame:frame device:(id<MTLDevice>)device];
    view.inputHandler = inputHandler;
    return (void*)view;
}

