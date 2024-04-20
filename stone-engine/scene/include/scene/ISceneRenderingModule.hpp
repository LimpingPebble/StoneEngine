// Copyright 2024 Stone-Engine

#pragma once

#include "SceneTypes.hpp"
#include <memory>

namespace Stone
{

    namespace Scene
    {

        class ISceneRenderingModule
        {
        public:

            virtual void generateDataForNode(std::shared_ptr<Node> node) {}
            virtual void generateDataForPivot(std::shared_ptr<Pivot> pivot) {}

            virtual void generateDataForCamera(std::shared_ptr<Camera> camera) {}
            virtual void generateDataForScene(std::shared_ptr<Scene> scene) {}

            virtual void generateDataForLight(std::shared_ptr<Light> light) {}
            virtual void generateDataForAmbientLight(std::shared_ptr<AmbientLight> ambientlight) {}
            virtual void generateDataForPointLight(std::shared_ptr<PointLight> pointlight) {}
            virtual void generateDataForCastingLight(std::shared_ptr<CastingLight> castinglight) {}
            virtual void generateDataForDirectionalLight(std::shared_ptr<DirectionalLight> directionallight) {}
            virtual void generateDataForSpotLight(std::shared_ptr<SpotLight> spotlight) {}

            virtual void generateDataForMesh(std::shared_ptr<Mesh> mesh) {}
            virtual void generateDataForInstancedMesh(std::shared_ptr<InstancedMesh> instancedmesh) {}
            virtual void generateDataForSkeleton(std::shared_ptr<Skeleton> skeleton) {}
            virtual void generateDataForSkinnedMesh(std::shared_ptr<SkinnedMesh> skinnedmesh) {}

            virtual void generateDataForSkybox(std::shared_ptr<Skybox> skybox) {}

            virtual void generateDataForTexture(std::shared_ptr<Texture> texture) {}
            virtual void generateDataForMaterial(std::shared_ptr<Material> material) {}

        };

    }
}
