#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <simd/simd.h>
#include <unordered_set>
#include <functional>

// Simple input state that can be queried each frame
class InputHandler
{
public:
    // Callbacks for immediate response (optional)
    std::function<void(unsigned short)> onKeyDown;
    std::function<void(unsigned short)> onKeyUp;
    std::function<void(int)> onMouseDown;  // 0=left, 1=right, 2=middle
    std::function<void(int)> onMouseUp;
    
    // Keyboard
    void keyDown(unsigned short keyCode);
    void keyUp(unsigned short keyCode);
    bool isKeyPressed(unsigned short keyCode) const;
    
    // Mouse position
    void setMousePosition(float x, float y);
    simd::float2 getMousePosition() const;
    
    // Mouse buttons (0 = left, 1 = right, 2 = middle)
    void mouseDown(int button);
    void mouseUp(int button);
    bool isMouseButtonPressed(int button) const;
    
    // Mouse delta (for camera control)
    void setMouseDelta(float dx, float dy);
    simd::float2 getMouseDelta() const;
    void resetMouseDelta(); // Call at end of frame
    
    // Scroll wheel
    void setScrollDelta(float dx, float dy);
    simd::float2 getScrollDelta() const;
    void resetScrollDelta();

private:
    std::unordered_set<unsigned short> _pressedKeys;
    simd::float2 _mousePosition = {0, 0};
    simd::float2 _mouseDelta = {0, 0};
    simd::float2 _scrollDelta = {0, 0};
    bool _mouseButtons[3] = {false, false, false};
};

// Common macOS key codes
namespace KeyCode
{
    constexpr unsigned short A = 0x00;
    constexpr unsigned short S = 0x01;
    constexpr unsigned short D = 0x02;
    constexpr unsigned short W = 0x0D;
    constexpr unsigned short Q = 0x0C;
    constexpr unsigned short E = 0x0E;
    constexpr unsigned short Space = 0x31;
    constexpr unsigned short Shift = 0x38;
    constexpr unsigned short Control = 0x3B;
    constexpr unsigned short Escape = 0x35;
    constexpr unsigned short LeftArrow = 0x7B;
    constexpr unsigned short RightArrow = 0x7C;
    constexpr unsigned short DownArrow = 0x7D;
    constexpr unsigned short UpArrow = 0x7E;
}

#endif //INPUTHANDLER_H

