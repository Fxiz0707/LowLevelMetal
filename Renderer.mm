#include "Renderer.h"

#include "MyUtility.h"
#include "MeshLoader.h"
#include "Math.h"

#include <iostream>
#include <simd/simd.h>

const int Renderer::kMaxFramesInFlight = 3;

Renderer::Renderer( MTL::Device* pDevice )
: _pDevice( pDevice->retain() ), _frame(0), _pRenderDataManager()
{
    _pCommandQueue = _pDevice->newCommandQueue();
    _semaphore = dispatch_semaphore_create(Renderer::kMaxFramesInFlight);
    buildShaders();
    buildBuffers();

    newSetup();
}

Renderer::~Renderer()
{
    _pVertexPositionBuffer->release();
    _pVertexColorsBuffer->release();
    _pPS0->release();
    _pShaderLibrary->release();
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

void Renderer::newSetup() { 
    Mesh tempMesh = MeshLoader::load_mesh("meshes/cow.obj", _pDevice);

    _pRenderDataManager.addMesh("cow", tempMesh);

    shader_types::InstanceData instanceData;
    instanceData.worldTransform = Math::identity();
    _pRenderDataManager.addInstance("cow", instanceData);

    _pRenderDataManager.setCameraTransform(Math::identity(), _frame);
    _pRenderDataManager.setProjectionTransform(Math::projection(0.5), _frame);

    // TEMP
    const Mesh& mesh = _pRenderDataManager.getMesh("cow"); 

    MTL::Buffer* pVertexPositionBuffer = _pDevice->newBuffer(mesh.mVertexData.data(), mesh.mVertexData.size() * sizeof(simd::float3), MTL::ResourceStorageModeShared);
    MTL::Buffer* pVertexIndexBuffer = _pDevice->newBuffer(mesh.mIndexData.data(), mesh.mIndexData.size() * sizeof(uint16_t), MTL::ResourceStorageModeShared);
    MTL::Buffer* pInstanceDataBuffer = _pDevice->newBuffer(_pRenderDataManager.getInstanceData("cow").data(), 
    _pRenderDataManager.getInstanceData("cow").size() * sizeof(shader_types::InstanceData), MTL::ResourceStorageModeShared);

    MTL::Buffer* pCameraTransformBuffer = _pDevice->newBuffer(sizeof(simd::float4x4), MTL::ResourceStorageModeShared);
    MTL::Buffer* pProjectionTransformBuffer = _pDevice->newBuffer(sizeof(simd::float4x3), MTL::ResourceStorageModeShared);

    *reinterpret_cast<simd::float4x4*>(pCameraTransformBuffer->contents()) = _pRenderDataManager.getCameraTransform(_frame);
    *reinterpret_cast<simd::float4x3*>(pProjectionTransformBuffer->contents()) = _pRenderDataManager.getProjectionTransform(_frame);

    _pVertexPositionBuffer = pVertexPositionBuffer;
    _pVertexIndexBuffer = pVertexIndexBuffer;
    _pInstanceDataBuffer = pInstanceDataBuffer;
    _pCameraTransformBuffer = pCameraTransformBuffer;
    _pProjectionTransformBuffer = pProjectionTransformBuffer;

    std::cout << "DURING SETUP" << std::endl;
    std::cout << "Vertex data size: " << mesh.mVertexData.size() << std::endl;
    std::cout << "Index data size: " << mesh.mIndexData.size() << std::endl;
    std::cout << "Instance data size: " << _pRenderDataManager.getInstanceData("cow").size() << std::endl;
    std::cout << "Camera transform buffer length: " << _pCameraTransformBuffer->length() << std::endl;
    std::cout << "Projection transform buffer length: " << _pProjectionTransformBuffer->length() << std::endl;
    std::cout << std::endl; 
}

void Renderer::draw( MTK::View* pView ) { 
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    _frame = (_frame + 1) % kMaxFramesInFlight;
    MTL::CommandBuffer *pCmd = _pCommandQueue->commandBuffer();

    dispatch_semaphore_wait(_semaphore, DISPATCH_TIME_FOREVER);
    Renderer* pRenderer = this;
    pCmd->addCompletedHandler(^void(MTL::CommandBuffer *pCmd)  {
        dispatch_semaphore_signal(pRenderer->_semaphore);
    });

    MTL::RenderPassDescriptor* pRpd = pView->currentRenderPassDescriptor();

    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder(pRpd);

    pEnc->setRenderPipelineState(_pPS0);

    // TESTING OUT

    pEnc->setVertexBuffer(_pVertexPositionBuffer, 0, 0);
    pEnc->setVertexBuffer(_pInstanceDataBuffer, 0, 1);
    pEnc->setVertexBuffer(_pCameraTransformBuffer, 0, 2);
    pEnc->setVertexBuffer(_pProjectionTransformBuffer, 0, 3);

    // draw as wireframe
    pEnc->setTriangleFillMode(MTL::TriangleFillMode::TriangleFillModeLines);

    pEnc->drawIndexedPrimitives( 
        MTL::PrimitiveType::PrimitiveTypeTriangle,
        _pVertexIndexBuffer->length() / sizeof(uint16_t), 
        MTL::IndexType::IndexTypeUInt16,
        _pVertexIndexBuffer,
        0,
        _pRenderDataManager.getInstanceData("cow").size()
    ); 

    // TESTING OUT END
    pEnc->endEncoding();

    pCmd->presentDrawable(pView->currentDrawable());
    pCmd->commit();
    pPool->release();
}