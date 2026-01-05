#include "RenderDataManager.h"

#include <stdexcept>


RenderDataManager::RenderDataManager() {};

void RenderDataManager::addMesh(std::string name, Mesh mesh) { 
    if (mMeshData.contains(name)) { 
        throw std::runtime_error("A mesh with that name already exists!");
    }
    mMeshData[name] = mesh;
}

void RenderDataManager::addInstance(std::string name, shader_types::InstanceData instanceData) { 
    if (!mMeshData.contains(name)) { 
        throw std::runtime_error("A mesh with that name does not exist!");
    }
    mInstanceData[name].push_back(instanceData);
}

void RenderDataManager::setCameraTransform(simd_float4x4 cameraTransform, int frame) { 
    mCameraTransform[frame] = cameraTransform;
}

void RenderDataManager::setProjectionTransform(simd_float4x3 projectionTransform, int frame) { 
    mProjectionTransform[frame] = projectionTransform;
}

const Mesh& RenderDataManager::getMesh(std::string name) const { 
    if (!mMeshData.contains(name)) { 
        throw std::runtime_error("A mesh with that name does not exist!");
    }
    return mMeshData.at(name);
}

const std::vector<shader_types::InstanceData>& RenderDataManager::getInstanceData(std::string name) const { 
    if (!mInstanceData.contains(name)) {
        throw std::runtime_error("A instance with that name does not exist!");
    }
    return mInstanceData.at(name);
}

simd_float4x4 RenderDataManager::getCameraTransform(int frame) const { 
    return mCameraTransform[frame];
}

simd_float4x3 RenderDataManager::getProjectionTransform(int frame) const { 
    return mProjectionTransform[frame];
}