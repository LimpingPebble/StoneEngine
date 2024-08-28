// Copyright 2024 Stone-Engine

#include "Core/Exceptions.hpp"
#include "Scene/AssetResource.hpp"
#include "Scene/Node/Node.hpp"
#include "Scene/Node/PivotNode.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/Node/SkinMeshNode.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/Renderable/SkinMesh.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Stone::Scene {

static std::unique_ptr<Assimp::Importer> _assimpImporter = nullptr;

static Assimp::Importer &getAssimpImporter() {
	if (!_assimpImporter) {
		_assimpImporter = std::make_unique<Assimp::Importer>();
	}
	return *_assimpImporter;
}

inline glm::vec3 convert(const aiVector3D &vector) {
	return {vector.x, vector.y, vector.z};
}

inline glm::vec2 convert(const aiVector2D &vector) {
    return {vector.x, vector.y};
}

inline glm::mat4 convert(const aiMatrix4x4 &matrix) {
    return {matrix.a1, matrix.a2, matrix.a3, matrix.a4,
                     matrix.b1, matrix.b2, matrix.b3, matrix.b4,
                     matrix.c1, matrix.c2, matrix.c3, matrix.c4,
                     matrix.d1, matrix.d2, matrix.d3, matrix.d4};
}

inline glm::quat convert(const aiQuaternion &quaternion) {
    return {quaternion.w, quaternion.x, quaternion.y, quaternion.z};
}

inline glm::vec3 convert(const aiColor3D &color) {
    return {color.r, color.g, color.b};
}

inline glm::vec4 convert(const aiColor4D &color) {
    return {color.r, color.g, color.b, color.a};
}

void loadMesh(AssetResource &AssetResource, const aiMesh *mesh) {
	std::shared_ptr<DynamicMesh> newMesh = std::make_shared<DynamicMesh>();

	newMesh->verticesRef().reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		newMesh->verticesRef().emplace_back(
            convert(mesh->mVertices[i]),
            convert(mesh->mNormals[i]),
            convert(mesh->mTangents[i]),
            convert(mesh->mBitangents[i]),
            mesh->mTextureCoords[0] ? convert(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f)
        );
	}

	newMesh->indicesRef().reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace &face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			newMesh->indicesRef().push_back(face.mIndices[j]);
		}
	}

    std::shared_ptr<StaticMesh> newStaticMesh = std::make_shared<StaticMesh>();
    newStaticMesh->setSourceMesh(newMesh);

	AssetResource.meshes.push_back(newMesh);
}

void loadSkinMesh(AssetResource &AssetResource, const aiMesh *mesh) {
	std::shared_ptr<DynamicSkinMesh> newMesh = std::make_shared<DynamicSkinMesh>();

    // TODO: Load skin mesh

	AssetResource.meshes.push_back(newMesh);
}

void loadMeshes(AssetResource &assetResource, const aiScene *scene) {
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[i];
        if (mesh->HasBones()) {
            loadSkinMesh(assetResource, mesh);
        } else {
            loadMesh(assetResource, mesh);
        }
	}
}

void loadTexture(AssetResource &assetResource, const aiTexture *texture) {
    auto image = assetResource.imagesAlbum->loadImage(texture->mFilename.C_Str());
    
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
    newTexture->setImage(image);

    assetResource.textures.push_back(newTexture);
}

void loadTextures(AssetResource &assetResource, const aiScene *scene) {
    for (unsigned int i = 0; i < scene->mNumTextures; i++) {
        const aiTexture *texture = scene->mTextures[i];
        loadTexture(assetResource, texture);
    }
}

void addMaterialScalar(
    const aiMaterial *material,
    const std::shared_ptr<Material> &newMaterial,
    const char *key,
    unsigned int type,
    unsigned int idx,
    const std::string &name) {

    float scalar;
    if (material->Get(key, type, idx, scalar) == AI_SUCCESS) {
        newMaterial->setScalarParameter(name, scalar);
    }
}

void addMaterialColor(
    const aiMaterial *material,
    const std::shared_ptr<Material> &newMaterial,
    const char *key,
    unsigned int type,
    unsigned int idx,
    const std::string &name) {

    aiColor3D color;
    if (material->Get(key, type, idx, color) == AI_SUCCESS) {
        newMaterial->setVectorParameter(name, convert(color));
    }
}

void addMaterialTexture(
    AssetResource &assetResource,
    const aiMaterial *material,
    const std::shared_ptr<Material> &newMaterial,
    aiTextureType type,
    const std::string &name) {

    if (material->GetTextureCount(type) <= 0)
        return;

    aiString texturePath;
    if (material->GetTexture(type, 0, &texturePath) != AI_SUCCESS)
        return;

    std::string texturePathStr = texturePath.C_Str();
    if (texturePathStr.empty())
        return;

    if (texturePathStr[0] == '*') {
        int textureIndex = std::stoi(texturePathStr.substr(1));
        if (textureIndex >= 0
            && textureIndex < static_cast<int>(assetResource.textures.size())) {
            std::shared_ptr<Texture> texture = assetResource.textures[textureIndex];
            newMaterial->setTextureParameter(name, texture);
        }
    } else {
        std::shared_ptr<Image::ImageSource> textureSource = assetResource.imagesAlbum->loadImage(texturePathStr);
        std::shared_ptr<Texture> texture = std::make_shared<Texture>();
        texture->setImage(textureSource);
        newMaterial->setTextureParameter(name, texture);
    }
}

void loadMaterial(AssetResource &assetResource, const aiMaterial *material) {
    std::shared_ptr<Material> newMaterial = std::make_shared<Material>();

    addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_DIFFUSE, "diffuse");
    addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_SPECULAR, "specular");
    addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_AMBIENT, "ambient");
    addMaterialColor(material, newMaterial, AI_MATKEY_COLOR_EMISSIVE, "emissive");
    addMaterialScalar(material, newMaterial, AI_MATKEY_SHININESS, "shininess");
    addMaterialScalar(material, newMaterial, AI_MATKEY_OPACITY, "opacity");

    addMaterialScalar(material, newMaterial, AI_MATKEY_ROUGHNESS_FACTOR, "roughness");
    addMaterialScalar(material, newMaterial, AI_MATKEY_METALLIC_FACTOR, "metallic");
    addMaterialScalar(material, newMaterial, AI_MATKEY_REFLECTIVITY, "reflectivity");

    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_DIFFUSE, "diffuse");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_SPECULAR, "specular");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_AMBIENT, "ambient");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_EMISSIVE, "emissive");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_HEIGHT, "height");   
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_NORMALS, "normals");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_SHININESS, "shininess");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_OPACITY, "opacity");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_DISPLACEMENT, "displacement");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_LIGHTMAP, "lightmap");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_REFLECTION, "reflection");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_BASE_COLOR, "color");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_NORMAL_CAMERA, "normal_camera");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_EMISSION_COLOR, "emission_color");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_METALNESS, "metalness");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_DIFFUSE_ROUGHNESS, "roughness");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_AMBIENT_OCCLUSION, "occlusion");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_SHEEN, "sheen");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_CLEARCOAT, "clearcoat");
    addMaterialTexture(assetResource, material, newMaterial, aiTextureType_TRANSMISSION, "transmission");

    assetResource.materials.push_back(newMaterial);
}

void loadMaterials(AssetResource &assetResource, const aiScene *scene) {
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial *material = scene->mMaterials[i];
        loadMaterial(assetResource, material);
    }
}

void loadNode(AssetResource &assetResource, const aiNode *node, const std::shared_ptr<PivotNode> &sceneNode) {

    sceneNode->getTransform().setMatrix(convert(node->mTransformation));

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        std::shared_ptr<PivotNode> childNode = std::make_shared<PivotNode>(node->mChildren[i]->mName.C_Str());
        sceneNode->addChild(childNode);
        loadNode(assetResource, node->mChildren[i], childNode);
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const unsigned int meshIndex = node->mMeshes[i];
        auto assetMesh = assetResource.meshes[meshIndex];
        assert(meshIndex < assetResource.meshes.size());

        if (auto asSkinMesh = std::dynamic_pointer_cast<ISkinMeshInterface>(assetMesh)) {
            std::shared_ptr<SkinMeshNode> skinMeshNode = std::make_shared<SkinMeshNode>("mesh_" + std::to_string(i));
            skinMeshNode->setSkinMesh(asSkinMesh);
            sceneNode->addChild(skinMeshNode);
        } else if (auto asMesh = std::dynamic_pointer_cast<IMeshInterface>(assetMesh)) {
            std::shared_ptr<MeshNode> meshNode = std::make_shared<MeshNode>("mesh_" + std::to_string(i));
            meshNode->setMesh(asMesh);
            sceneNode->addChild(meshNode);
        }
    }

}

std::shared_ptr<Node> Node::load(const std::string &path) {
	Assimp::Importer &importer = getAssimpImporter();

	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
													   aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

    // Additional flags:
    // aiProcess_OptimizeMeshes
    // aiProcess_SplitLargeMeshes

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw Core::FileLoadingError(path, importer.GetErrorString());
	}

	AssetResource assetResource(path);

    assetResource.rootNode = std::make_shared<PivotNode>(scene->mRootNode->mName.C_Str());

	std::cout << "scene "
			  << "validated: " << (scene->mFlags & AI_SCENE_FLAGS_VALIDATED) << " | "
			  << "incomplete: " << (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) << " | "
			  << "mNumMeshes " << scene->mNumMeshes << " | "
			  << "mNumMaterials " << scene->mNumMaterials << " | "
			  << "mNumAnimations " << scene->mNumAnimations << " | "
			  << "mNumTextures " << scene->mNumTextures << " | "
			  << "mNumLights " << scene->mNumLights << " | "
			  << "mNumCameras " << scene->mNumCameras << " | "
			  << "mNumSkeletons " << scene->mNumSkeletons << " | "
			  << "mMetaData " << scene->mMetaData << " | "
			  << "mName" << scene->mName.C_Str() << " | " << std::endl;

    /*
    * TODO:
    * load:
    * - meshes
    *   - vertices
    *   - indices
    *   - tangents
    *   - bitangents
    *   - uv
    * - skin meshes
    *   - bones
    * - materials
    * - animations
    * - textures
    * - lights
    * - cameras
    * - skeletons
    * create:
    * - nodes
    * - pivot nodes
    * - mesh nodes
    * - skin mesh nodes
    */


	loadMeshes(assetResource, scene);
    loadTextures(assetResource, scene);
    loadMaterials(assetResource, scene);

    loadNode(assetResource, scene->mRootNode, assetResource.rootNode);

	return assetResource.rootNode;
}

} // namespace Stone::Scene
