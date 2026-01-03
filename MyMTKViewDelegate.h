//
// Created by Abdullah Faiz ur rahman on 22/12/2025.
//

#ifndef MYMTKVIEWDELEGATE_H
#define MYMTKVIEWDELEGATE_H


#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>

#include <MetalKit/MetalKit.hpp>

#include "Renderer.h"

class MyMTKViewDelegate : public MTK::ViewDelegate
{
    public:
        MyMTKViewDelegate( MTL::Device* pDevice );
        virtual ~MyMTKViewDelegate() override;
        virtual void drawInMTKView( MTK::View* pView ) override;

    private:
        Renderer* _pRenderer;
};

#endif //MYMTKVIEWDELEGATE_H
