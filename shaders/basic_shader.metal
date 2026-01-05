#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

constant float4x4 identityMatrix = float4x4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
);

struct InstanceData { 
    float4x4 worldTransform;
};

v2f vertex vertexMain(
    device const float3* positions [[buffer(0)]],
    device const InstanceData* instanceData [[buffer(1)]],
    device const float4x4& cameraTransform [[buffer(2)]],
    device const float4x3& projectionTransform [[buffer(3)]],
    uint vertexId [[vertex_id]],
    uint instanceId [[instance_id]]
)
{
    v2f o;
    o.position = float4( positions[ vertexId ], 5.0 );
    //o.position = identityMatrix * o.position;
    //o.color = half3(colors[vertexId]);
    o.color = half3(0.1, 0.9, 0.2);
    return o;
}

half4 fragment fragmentMain(v2f in [[stage_in]])
{
    return half4(in.color, 1.0);
}