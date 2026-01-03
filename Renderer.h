//
// Created by Abdullah Faiz ur rahman on 22/12/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "MyUtility.h"

#include <iostream>

#include <Metal/Metal.hpp>
#include "Metal/MTLResource.hpp"
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <simd/simd.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "MeshLoader.h"

class Renderer
{
    public:
        Renderer( MTL::Device* pDevice );
        ~Renderer();
        void buildShaders();
        void buildBuffers();
        void draw( MTK::View* pView );

    private:
        MTL::Device* _pDevice;

        MTL::Library* _pShaderLibrary;
        MTL::RenderPipelineState* _pPS0; // idk if there is some attaching that happens with it...

        MTL::CommandQueue* _pCommandQueue;
        MTL::Buffer *_pVertexPositionBuffer;
        MTL::Buffer *_pVertexColorsBuffer;
        MTL::Buffer* _pArgBuffer;

        // dont really see how these are used:
        dispatch_semaphore_t _semaphore;
        static const int kMaxFramesInFlight;
};

#endif //RENDERER_H
