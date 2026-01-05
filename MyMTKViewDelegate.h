#ifndef MYMTKVIEWDELEGATE_H
#define MYMTKVIEWDELEGATE_H

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "Renderer.h"
#include "InputHandler.h"

class MyMTKViewDelegate : public MTK::ViewDelegate
{
    public:
        MyMTKViewDelegate( MTL::Device* pDevice, InputHandler* pInput );
        virtual ~MyMTKViewDelegate() override;
        virtual void drawInMTKView( MTK::View* pView ) override;

    private:
        void update( float deltaTime );
        
        Renderer* _pRenderer;
        InputHandler* _pInput;
};

#endif //MYMTKVIEWDELEGATE_H