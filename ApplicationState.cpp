#include "ApplicationState.h"

ApplicationState::ApplicationState(MTL::Device* pDevice) : _pDevice(pDevice->retain()) { 
}

ApplicationState::~ApplicationState() { 
    _pDevice->release();
    _pPS0->release();
}

void ApplicationState::init() {
}

void ApplicationState::buildShaders() { 
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
    pLibrary->release();
}

void ApplicationState::buildBuffers() { 

}

void ApplicationState::setCameraMatrix()  { 

}