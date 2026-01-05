#include "InputHandler.h"

void InputHandler::keyDown(unsigned short keyCode)
{
    _pressedKeys.insert(keyCode);
    if (onKeyDown) onKeyDown(keyCode);
}

void InputHandler::keyUp(unsigned short keyCode)
{
    _pressedKeys.erase(keyCode);
    if (onKeyUp) onKeyUp(keyCode);
}

bool InputHandler::isKeyPressed(unsigned short keyCode) const
{
    return _pressedKeys.count(keyCode) > 0;
}

void InputHandler::setMousePosition(float x, float y)
{
    _mousePosition = {x, y};
}

simd::float2 InputHandler::getMousePosition() const
{
    return _mousePosition;
}

void InputHandler::mouseDown(int button)
{
    if (button >= 0 && button < 3) {
        _mouseButtons[button] = true;
        if (onMouseDown) onMouseDown(button);
    }
}

void InputHandler::mouseUp(int button)
{
    if (button >= 0 && button < 3) {
        _mouseButtons[button] = false;
        if (onMouseUp) onMouseUp(button);
    }
}

bool InputHandler::isMouseButtonPressed(int button) const
{
    if (button >= 0 && button < 3)
        return _mouseButtons[button];
    return false;
}

void InputHandler::setMouseDelta(float dx, float dy)
{
    _mouseDelta.x += dx;
    _mouseDelta.y += dy;
}

simd::float2 InputHandler::getMouseDelta() const
{
    return _mouseDelta;
}

void InputHandler::resetMouseDelta()
{
    _mouseDelta = {0, 0};
}

void InputHandler::setScrollDelta(float dx, float dy)
{
    _scrollDelta.x += dx;
    _scrollDelta.y += dy;
}

simd::float2 InputHandler::getScrollDelta() const
{
    return _scrollDelta;
}

void InputHandler::resetScrollDelta()
{
    _scrollDelta = {0, 0};
}

