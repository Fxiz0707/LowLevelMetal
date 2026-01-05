#ifndef MESH_H
#define MESH_H

#include <simd/simd.h>

#include <string>
#include <vector>

struct Mesh { 
    std::string name;
    std::vector<simd::float3> mVertexData;
    std::vector<uint16_t> mIndexData;
};

#endif //MESH_H
