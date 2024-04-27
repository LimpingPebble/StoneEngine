// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RenderContext.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Vertex.hpp"

namespace Stone::Scene {

/** Node */
class Node;
class PivotNode;
class CameraNode;
class LightNode;
class AmbientLightNode;
class PointLightNode;
class CastingLightNode;
class DirectionalLightNode;
class SpotLightNode;
class MeshNode;
class InstancedMeshNode;
class SkeletonNode;
class SkinMeshNode;
class WorldNode;

/** RenderElement */
class Texture;
class Material;

class Mesh;
class Skeleton;
class SkinMesh;

} // namespace Stone::Scene
