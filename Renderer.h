#ifndef RENDERER_H
#define RENDERER_H

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "RenderDataManager.h"

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

        MTL::Library* _pShaderLibrary; // remove
        MTL::RenderPipelineState* _pPS0;  // remove

        MTL::CommandQueue* _pCommandQueue;
        MTL::Buffer *_pVertexPositionBuffer; // remove
        MTL::Buffer *_pVertexColorsBuffer; // remove

        int _frame;

        // dont really see how these are used:
        dispatch_semaphore_t _semaphore;
        static const int kMaxFramesInFlight;

        // TESTING OUT
        void newSetup();
        RenderDataManager _pRenderDataManager;

        // MTL::Buffer *_pVertexPositionBuffer;
        MTL::Buffer *_pVertexIndexBuffer;
        MTL::Buffer *_pInstanceDataBuffer;
        MTL::Buffer *_pCameraTransformBuffer;
        MTL::Buffer *_pProjectionTransformBuffer;
};

#endif //RENDERER_H