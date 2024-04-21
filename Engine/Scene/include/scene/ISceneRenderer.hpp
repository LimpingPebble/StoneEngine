// Copyright 2024 Stone-Engine

#pragma once

#include "SceneTypes.hpp"
#include <memory>

namespace Stone
{

    namespace Scene
    {

        class ISceneRenderer
        {
        public:

            virtual void generateDataForMesh(std::shared_ptr<Mesh> mesh) = 0;
            virtual void generateDataForInstancedMesh(std::shared_ptr<InstancedMesh> instancedmesh) = 0;
            virtual void generateDataForSkinnedMesh(std::shared_ptr<SkinnedMesh> skinnedmesh) = 0;

            virtual void generateDataForSkybox(std::shared_ptr<Skybox> skybox) = 0;

            virtual void generateDataForTexture(std::shared_ptr<Texture> texture) = 0;
            virtual void generateDataForMaterial(std::shared_ptr<Material> material) = 0;

        };

    }
}
