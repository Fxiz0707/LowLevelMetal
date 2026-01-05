#include "simd/simd.h"

namespace shader_types {

    struct InstanceData { 
        simd_float4x4 worldTransform;
    };

};