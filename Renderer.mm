#include "Renderer.h"

const int Renderer::kMaxFramesInFlight = 3;

Renderer::Renderer( MTL::Device* pDevice )
: _pDevice( pDevice->retain() )
{
    _pCommandQueue = _pDevice->newCommandQueue();
    _semaphore = dispatch_semaphore_create(Renderer::kMaxFramesInFlight);
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer()
{
    _pCommandQueue->release();
    _pDevice->release();
}

void Renderer::buildShaders() {
    using NS::StringEncoding::UTF8StringEncoding;

    std::string shader_string = MyUtility::read_file("shaders/basic_shader.metal");
    NS::Error *pError = nullptr;
    MTL::Library* pLibrary = _pDevice->newLibrary(
        NS::String::string(shader_string.c_str(), UTF8StringEncoding), nullptr, &pError
    );
    if ( !pLibrary ) {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    MTL::Function* pVertexFn = pLibrary->newFunction(NS::String::string("vertexMain", NS::UTF8StringEncoding));
    if (!pVertexFn) {
        __builtin_printf("Failed to find vertex function 'vertexMain'\n");
        assert(false);
    }

    MTL::Function* pFragFn = pLibrary->newFunction(NS::String::string("fragmentMain", NS::UTF8StringEncoding));
    if (!pFragFn) {
        __builtin_printf("Failed to find fragment function 'fragmentMain'\n");
        assert(false);
    }

    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();

    pDesc->setVertexFunction( pVertexFn );
    pDesc->setFragmentFunction( pFragFn );
    pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

    _pPS0 = _pDevice->newRenderPipelineState( pDesc, &pError );
    if ( !_pPS0 )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    pVertexFn->release();
    pFragFn->release();
    pDesc->release();
    _pShaderLibrary = pLibrary;
}

void Renderer::buildBuffers() {
    using NS::StringEncoding::UTF8StringEncoding;

    const size_t NumVertices = 3;
    
    simd::float3 positions[NumVertices] =
    {
        { -0.8f,  0.8f, 0.0f },
        {  0.0f, -0.8f, 0.0f },
        { +0.8f,  0.8f, 0.0f }
    };
    
    simd::float3 colors[NumVertices] =  
    { 
        {1.0, 0.3f, 0.2f},
        {0.8f, 1.0, 0.0f},
        {0.8f, 0.0f, 1.0} 
    };

    const size_t positionsDataSize = NumVertices * sizeof(simd::float3);
    const size_t colorDataSize = NumVertices * sizeof(simd::float3);

    _pVertexPositionBuffer = _pDevice->newBuffer(positions, positionsDataSize, MTL::ResourceStorageModeShared);
    _pVertexColorsBuffer = _pDevice->newBuffer(colors, colorDataSize, MTL::ResourceStorageModeShared);
}

void Renderer::draw( MTK::View* pView )
{
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();

    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    Renderer* pRenderer = this;
    pCmd->addCompletedHandler(^void(MTL::CommandBuffer *pCmd)  {
        dispatch_semaphore_signal(pRenderer->_semaphore);
    });

    MTL::RenderPassDescriptor* pRpd = pView->currentRenderPassDescriptor();

    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder(pRpd);

    pEnc->setRenderPipelineState(_pPS0);
    pEnc->setVertexBuffer(_pVertexPositionBuffer, 0, 0);
    pEnc->setVertexBuffer(_pVertexColorsBuffer, 0, 1);

    pEnc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    pEnc->endEncoding();


    pCmd->presentDrawable(pView->currentDrawable());
    pCmd->commit();

    pPool->release();
}