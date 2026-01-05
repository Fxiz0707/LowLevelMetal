#ifndef RENDER_DATA_MANAGER_H
#define RENDER_DATA_MANAGER_H

#include <simd/simd.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "Mesh.h"
#include "ShaderTypes.h"

class RenderDataManager { 
    public:
        RenderDataManager();

        void addMesh(std::string name, Mesh mesh);
        void addInstance(std::string name, shader_types::InstanceData);
        void setCameraTransform(simd_float4x4 cameraTransform, int frame);
        void setProjectionTransform(simd_float4x3 projectionTransform, int frame);

        const Mesh& getMesh(std::string name) const;
        const std::vector<shader_types::InstanceData>& getInstanceData(std::string name) const;
        simd_float4x4 getCameraTransform(int frame) const;
        simd_float4x3 getProjectionTransform(int frame) const;

    private:
        std::unordered_map<std::string, Mesh> mMeshData;
        std::unordered_map<std::string, std::vector<shader_types::InstanceData>> mInstanceData;
        simd_float4x4 mCameraTransform[3]; 
        simd_float4x3 mProjectionTransform[3];
};

#endif //RENDER_DATA_MANAGER_H