#include "MyMTKViewDelegate.h"
#include <cstdio>

// Helper to get key name
static const char* getKeyName(unsigned short keyCode)
{
    switch (keyCode) {
        case KeyCode::W: return "W";
        case KeyCode::A: return "A";
        case KeyCode::S: return "S";
        case KeyCode::D: return "D";
        case KeyCode::Q: return "Q";
        case KeyCode::E: return "E";
        case KeyCode::Space: return "SPACE";
        case KeyCode::Shift: return "SHIFT";
        case KeyCode::Control: return "CTRL";
        case KeyCode::Escape: return "ESC";
        case KeyCode::UpArrow: return "UP";
        case KeyCode::DownArrow: return "DOWN";
        case KeyCode::LeftArrow: return "LEFT";
        case KeyCode::RightArrow: return "RIGHT";
        default: return nullptr;
    }
}

MyMTKViewDelegate::MyMTKViewDelegate( MTL::Device* pDevice, InputHandler* pInput )
: MTK::ViewDelegate()
, _pRenderer( new Renderer( pDevice ) )
, _pInput( pInput )
{
    // Set up instant key callbacks for debugging
    _pInput->onKeyDown = [](unsigned short keyCode) {
        const char* name = getKeyName(keyCode);
        if (name) {
            printf("[KEY DOWN] %s\n", name);
        } else {
            printf("[KEY DOWN] code=%d\n", keyCode);
        }
    };
    
    _pInput->onKeyUp = [](unsigned short keyCode) {
        const char* name = getKeyName(keyCode);
        if (name) {
            printf("[KEY UP]   %s\n", name);
        } else {
            printf("[KEY UP]   code=%d\n", keyCode);
        }
    };
    
    // Mouse click callbacks
    _pInput->onMouseDown = [](int button) {
        const char* names[] = {"LEFT", "RIGHT", "MIDDLE"};
        printf("[MOUSE DOWN] %s\n", names[button]);
    };
    
    _pInput->onMouseUp = [](int button) {
        const char* names[] = {"LEFT", "RIGHT", "MIDDLE"};
        printf("[MOUSE UP]   %s\n", names[button]);
    };
}

MyMTKViewDelegate::~MyMTKViewDelegate()
{
    delete _pRenderer;
}

void MyMTKViewDelegate::update( float deltaTime )
{
    // Key events are now printed instantly via callbacks
    // Here you would use isKeyPressed() for continuous actions:
    //
    // if (_pInput->isKeyPressed(KeyCode::W)) {
    //     camera.moveForward(deltaTime * speed);
    // }
    
    // Mouse delta - only print if non-zero
    simd::float2 mouseDelta = _pInput->getMouseDelta();
    if (mouseDelta.x != 0 || mouseDelta.y != 0)
    {
        printf("Mouse delta: (%.2f, %.2f)\n", mouseDelta.x, mouseDelta.y);
    }
    
    // Scroll delta
    simd::float2 scroll = _pInput->getScrollDelta();
    if (scroll.x != 0 || scroll.y != 0)
    {
        printf("Scroll: (%.2f, %.2f)\n", scroll.x, scroll.y);
    }
}

void MyMTKViewDelegate::drawInMTKView( MTK::View* pView )
{
    // 1. Process input & update game state
    float dt = 1.0f / 60.0f;
    update( dt );
    
    // 2. Render
    _pRenderer->draw( pView );
    
    // 3. Reset per-frame input deltas
    _pInput->resetMouseDelta();
    _pInput->resetScrollDelta();
}
