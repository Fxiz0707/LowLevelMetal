#ifndef INPUTMTKVIEW_H
#define INPUTMTKVIEW_H

#include "InputHandler.h"

#ifdef __OBJC__
#import <MetalKit/MetalKit.h>

// Custom MTKView subclass that captures input events
@interface InputMTKView : MTKView

@property (nonatomic, assign) InputHandler* inputHandler;

@end

#endif // __OBJC__

// C++ interface for creating the view (callable from .cpp files)
#ifdef __cplusplus
extern "C" {
#endif

void* CreateInputMTKView(CGRect frame, void* device, InputHandler* inputHandler);

#ifdef __cplusplus
}
#endif

#endif //INPUTMTKVIEW_H

