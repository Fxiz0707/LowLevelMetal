#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <Metal/Metal.hpp>
#include <simd/simd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "Mesh.h"

// loads meshes from .obj files
struct MeshLoader { 

    /*
        for now we are only going to just focus on v and f 
        and it only works for faces that dont have the / (no tex coords, no normal coords etc)
    */
    static Mesh load_mesh(std::string fullPath, MTL::Device* pDevice) { 
        std::filesystem::path pathObj(fullPath);
        std::string fileName = std::filesystem::path(fullPath).stem().string();

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
                uint16_t i1 = std::stoi(std::string((*it).begin(), (*it).end())) - 1;
                it++;
                uint16_t i2 = std::stoi(std::string((*it).begin(), (*it).end())) - 1;
                it++;
                uint16_t i3 = std::stoi(std::string((*it).begin(), (*it).end())) - 1;
                
                index_vector.push_back(i1);
                index_vector.push_back(i2);
                index_vector.push_back(i3);
            }
        }

        Mesh rv;
        rv.name = fileName;
        rv.mVertexData = vertex_vector; 
        rv.mIndexData = index_vector; 

        return rv;
    };
};

#endif //MESH_LOADER_H
