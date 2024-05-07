#include "Scene.hpp"

#include <gtest/gtest.h>

using namespace Stone::Scene;

TEST(Scene, Nodes) {
	// Create a world node
	std::shared_ptr<WorldNode> world = WorldNode::create();

	// Create some assets
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	std::shared_ptr<Texture> blank_texture = std::make_shared<Texture>();
	std::shared_ptr<Material> blank_material = std::make_shared<Material>();
	blank_material->setTextureParameter("diffuse", blank_texture);

	// Create child nodes
	auto child1 = world->addChild<PivotNode>("child1");
	child1->getTransform().translate(glm::vec3(2.0f, 2.0f, 0.0f));

	auto child2 = child1->addChild<PivotNode>("child2");

	child2->getTransform().translate(glm::vec3(4.0f, 4.0f, 0.0f));

	auto child2Mesh = child2->addChild<MeshNode>("mesh");
	child2Mesh->setMesh(mesh);
	child2Mesh->setMaterial(blank_material);

	EXPECT_EQ(child2Mesh->getMesh(), mesh);
	EXPECT_EQ(child2Mesh->getMaterial(), blank_material);
	EXPECT_EQ(child2Mesh->getMaterial()->getTextureParameter("diffuse"), blank_texture);

	EXPECT_EQ(child2Mesh->getGlobalName(), "/world/child1/child2/mesh");
	EXPECT_EQ(child2Mesh->getParent(), child2);

	auto worldMatrix = child2Mesh->getWorldTransformMatrix();
	EXPECT_NEAR(worldMatrix[3][0], 6.0f, 0.0001f);
	EXPECT_NEAR(worldMatrix[3][1], 6.0f, 0.0001f);
	EXPECT_NEAR(worldMatrix[3][2], 0.0f, 0.0001f);

	EXPECT_TRUE(child2Mesh->isDescendantOf(child1));
	EXPECT_TRUE(child1->isAncestorOf(child2Mesh));

	EXPECT_EQ(child2Mesh->getWorld(), world);

	auto child3 = world->addChild<InstancedMeshNode>("child3");
	auto child4 = child3->addChild<AmbientLightNode>("child4");
	auto child5 = child4->addChild<PointLightNode>();
	auto child6 = child4->addChild<DirectionalLightNode>();
	auto child7 = child6->addChild<SpotLightNode>("child7");

	EXPECT_EQ(std::string(world->getChildByPath("*/child7")->getClassName()), "SpotLightNode");

	std::shared_ptr<PerspectiveCameraNode> child1_1 = child1->addChild<PerspectiveCameraNode>("child1_1");
	world->setActiveCamera(child1_1);

	EXPECT_EQ(world->getActiveCamera(), child1_1);

	std::shared_ptr<Node> child1_2 = child1->addChild<Node>("child1_2");
	std::shared_ptr<Node> child1_3 = child1->addChild<OrthographicCameraNode>("child1_3");
	std::shared_ptr<Node> child1_4 = child1->addChild<PivotNode>("child1_4");

	EXPECT_EQ(child1->getChildByClass<OrthographicCameraNode>(), child1_3);

	std::shared_ptr<PivotNode> human = world->addChild<PivotNode>("human");
	std::shared_ptr<SkinMeshNode> humanMesh = human->addChild<SkinMeshNode>("human_mesh");
	std::shared_ptr<SkeletonNode> skeleton = humanMesh->addChild<SkeletonNode>("skeleton");
	std::shared_ptr<PivotNode> spine = skeleton->addChild<PivotNode>("spine");
	std::shared_ptr<PivotNode> chest = spine->addChild<PivotNode>("chest");
	std::shared_ptr<PivotNode> elbow = chest->addChild<PivotNode>("elbow");
	std::shared_ptr<PivotNode> arm = elbow->addChild<PivotNode>("arm");
	std::shared_ptr<PivotNode> hand = arm->addChild<PivotNode>("hand");
	std::shared_ptr<PointLightNode> torch = hand->addChild<PointLightNode>("torch");

	chest->getTransform().translate(glm::vec3(0.0f, 1.0f, 0.0f));
	elbow->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
	arm->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
	arm->getTransform().rotate(glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f))));
	hand->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
	skeleton->addBone(spine);
	skeleton->addBone(chest);
	skeleton->addBone(elbow);
	skeleton->addBone(arm);
	skeleton->addBone(hand);
	humanMesh->setSkeleton(skeleton);

	EXPECT_EQ(humanMesh->getSkeleton(), skeleton);

	glm::mat4 t = hand->getTransformMatrixRelativeToNode(elbow);

	EXPECT_NEAR(t[3][0], 1.707107f, 0.0001f);
	EXPECT_NEAR(t[3][1], 0.707107f, 0.0001f);
	EXPECT_NEAR(t[3][2], 0.0f, 0.0001f);
}

template <typename T>
std::shared_ptr<T> makeNode(const std::string &type, const std::string &name) {
	auto &factory = Stone::DynamicObjectFactory<Node, const std::string &>::getInstance();
	std::shared_ptr<Node> node = factory.create(type, name);
	return std::dynamic_pointer_cast<T>(node);
}

void testNodeDynamic() {
	auto world = makeNode<WorldNode>("WorldNode", "world");
	EXPECT_EQ(world->getClassName(), "WorldNode");
	auto child1 = makeNode<PivotNode>("PivotNode", "child1");
	EXPECT_EQ(child1->getClassName(), "PivotNode");
	world->addChild(child1);
	auto child2 = makeNode<MeshNode>("MeshNode", "child2");
	EXPECT_EQ(child2->getClassName(), "MeshNode");
	world->addChild(child2);
	auto child3 = makeNode<InstancedMeshNode>("InstancedMeshNode", "child3");
	EXPECT_EQ(child3->getClassName(), "InstancedMeshNode");
	world->addChild(child3);
	auto child4 = makeNode<AmbientLightNode>("AmbientLightNode", "child4");
	EXPECT_EQ(child4->getClassName(), "AmbientLightNode");
	world->addChild(child4);
	auto child5 = makeNode<PointLightNode>("PointLightNode", "child5");
	EXPECT_EQ(child5->getClassName(), "PointLightNode");
	child4->addChild(child5);
	auto child6 = makeNode<DirectionalLightNode>("DirectionalLightNode", "child6");
	EXPECT_EQ(child6->getClassName(), "DirectionalLightNode");
	child4->addChild(child6);
	auto child7 = makeNode<SpotLightNode>("SpotLightNode", "child7");
	EXPECT_EQ(child7->getClassName(), "SpotLightNode");
	child6->addChild(child7);
	auto child1_1 = makeNode<PerspectiveCameraNode>("PerspectiveCameraNode", "child1_1");
	EXPECT_EQ(child1_1->getClassName(), "PerspectiveCameraNode");
	child1->addChild(child1_1);
	auto child1_2 = makeNode<OrthographicCameraNode>("OrthographicCameraNode", "child1_2");
	EXPECT_EQ(child1_2->getClassName(), "OrthographicCameraNode");
	child1->addChild(child1_2);
	auto child1_3 = makeNode<Node>("Node", "child1_3");
	EXPECT_EQ(child1_3->getClassName(), "Node");
	child1->addChild(child1_3);
	auto human = makeNode<PivotNode>("PivotNode", "human");
	EXPECT_EQ(human->getClassName(), "PivotNode");
	world->addChild(human);
	auto humanMesh = makeNode<SkinMeshNode>("SkinMeshNode", "human_mesh");
	EXPECT_EQ(humanMesh->getClassName(), "SkinMeshNode");
	human->addChild(humanMesh);
	auto skeleton = makeNode<SkeletonNode>("SkeletonNode", "skeleton");
	EXPECT_EQ(skeleton->getClassName(), "SkeletonNode");
	humanMesh->addChild(skeleton);
	auto spine = makeNode<PivotNode>("PivotNode", "spine");
	EXPECT_EQ(spine->getClassName(), "PivotNode");
	skeleton->addChild(spine);
	auto chest = makeNode<PivotNode>("PivotNode", "chest");
	EXPECT_EQ(chest->getClassName(), "PivotNode");
	spine->addChild(chest);
	auto elbow = makeNode<PivotNode>("PivotNode", "elbow");
	EXPECT_EQ(elbow->getClassName(), "PivotNode");
	chest->addChild(elbow);
	auto arm = makeNode<PivotNode>("PivotNode", "arm");
	EXPECT_EQ(arm->getClassName(), "PivotNode");
	elbow->addChild(arm);
	auto hand = makeNode<PivotNode>("PivotNode", "hand");
	EXPECT_EQ(hand->getClassName(), "PivotNode");
	arm->addChild(hand);
	auto torch = makeNode<PointLightNode>("PointLightNode", "torch");
	EXPECT_EQ(torch->getClassName(), "PointLightNode");
	hand->addChild(torch);

	auto none = makeNode<Node>("Node", "none");
	EXPECT_EQ(none, nullptr);
}
