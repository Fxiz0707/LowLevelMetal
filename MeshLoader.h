

// #include "Metal/MTLBuffer.hpp"
#include <Metal/Metal.hpp>
#include <simd/simd.h>

#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <ranges>
#include <unordered_map>

#include "Mesh.h"
#include "Metal/MTLResource.hpp"


// loads meshes from .obj files
struct MeshLoader { 

    /*
        for now we are only going to just focus on v and f 
        and it only works for faces that dont have the / (no tex coords, no normal coords etc)
    */
    static Mesh load_mesh(std::string fullPath, MTL::Device* pDevice) { 
        std::ifstream fin(fullPath);

        std::vector<simd::float3> vertex_vector;
        std::vector<uint16_t> index_vector;

        std::string line;
        while (std::getline(fin, line)) {  

            auto split = std::views::split(line, ' ');
            auto it = split.begin();

            auto range1 = *it;
            std::string_view category(range1.begin(), range1.end());

            if (category == "v") { // dealing with vertex data
                it++;
                float v1 = std::stof(std::string((*it).begin(), (*it).end()));
                it++;
                float v2 = std::stof(std::string((*it).begin(), (*it).end()));
                it++;
                float v3 = std::stof(std::string((*it).begin(), (*it).end()));

                simd::float3 vec3 = {v1, v2, v3};
                vertex_vector.push_back(vec3);
            }
            else if (category == "f") // dealing with index data
            { 
                it++;
                uint16_t i1 = std::stoi(std::string((*it).begin(), (*it).end()));
                it++;
                uint16_t i2 = std::stoi(std::string((*it).begin(), (*it).end()));
                it++;
                uint16_t i3 = std::stof(std::string((*it).begin(), (*it).end()));
                
                index_vector.push_back(i1);
                index_vector.push_back(i2);
                index_vector.push_back(i3);
            }
        }
        
        Mesh rv;
        rv.vertexBuffer = pDevice->newBuffer(&vertex_vector[0], sizeof(simd::float3) * vertex_vector.size(), MTL::ResourceStorageModeShared);
        rv.indexBuffer = pDevice->newBuffer(&index_vector[0], sizeof(uint16_t) * index_vector.size(), MTL::ResourceStorageModeShared);

        return rv;
    };
};