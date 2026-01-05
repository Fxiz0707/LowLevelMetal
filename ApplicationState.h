#include <Metal/Metal.hpp>

#include "MyUtility.h"

struct ApplicationState { 
    ApplicationState(MTL::Device* pDevice);
    ~ApplicationState();
    void init();
    void buildShaders();
    void buildBuffers();

    void setCameraMatrix(); // not sure on the name yet

    MTL::Device* _pDevice;
    MTL::RenderPipelineState* _pPS0;
};