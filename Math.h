#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <simd/simd.h>

struct Math { 
    simd::float4x4 static inline identity() { 
        simd::float4 col0 = {1.f, 0.f, 0.f, 0.f}; 
        simd::float4 col1 = {0.f, 1.f, 0.f, 0.f}; 
        simd::float4 col2 = {0.f, 0.f, 1.f, 0.f}; 
        simd::float4 col3 = {0.f, 0.f, 0.f, 1.f}; 
        return simd::float4x4(col0, col1, col2, col3);
    };

    simd::float4x4 static inline rotateYaw(float alpha) { 
        simd::float4 col0 = {cos(alpha), -sin(alpha), 0.f, 0.f}; 
        simd::float4 col1 = {sin(alpha), cos(alpha), 0.f, 0.f}; 
        simd::float4 col2 = {0.f, 0.f, 1.f, 0.f}; 
        simd::float4 col3 = {0.f, 0.f, 0.f, 1.f}; 
        return simd::float4x4(col0, col1, col2, col3);
    }

    simd::float4x4 static inline rotatePitch(float alpha) { 
        simd::float4 col0 = {cos(alpha), 0.f, 0.f, sin(alpha)}; 
        simd::float4 col1 = {0.f, 1.f, 0.f, 0.f}; 
        simd::float4 col2 = {-sin(alpha), 0.f, 1.f, cos(alpha)}; 
        simd::float4 col3 = {0.f, 0.f, 0.f, 1.f}; 
        return simd::float4x4(col0, col1, col2, col3);
    };

    simd::float4x4 static inline translate(float x, float y, float z) { 
        simd::float4 col0 = {1.f, 0.f, 0.f, x}; 
        simd::float4 col1 = {0.f, 1.f, 0.f, y}; 
        simd::float4 col2 = {0.f, 0.f, 1.f, z}; 
        simd::float4 col3 = {0.f, 0.f, 0.f, 1.f}; 
        return simd::float4x4(col0, col1, col2, col3);
    };

    simd::float4x3 static inline projection(float f) {
        simd::float3 col0 = {f, 0.f, 0.f}; 
        simd::float3 col1 = {0.f, f, 0.f}; 
        simd::float3 col2 = {0.f, 0.f, 1.f};
        simd::float3 col3 = {0.f, 0.f, 0.f};
        return simd::float4x3(col0, col1, col2, col3);
    }
};

#endif //MATH_H