#include <iostream>
#include <cassert>
#include "Scene.hpp"

using namespace Stone::Scene;

template <typename T>
std::shared_ptr<T> makeNode(const std::string &type, const std::string &name)
{
    return std::dynamic_pointer_cast<T>(DynamicObjectFactory<Node, const std::string &>::getInstance()
        .createObject(type, name));
}

void testNode()
{
    // Create a root node
    std::shared_ptr<Scene> rootNode = std::make_shared<Scene>("root");

    std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>("skybox");
    rootNode->addChild(skybox);

    std::shared_ptr<Texture> blank_texture = std::make_shared<Texture>();
    skybox->setTexture(blank_texture, 0);
    // skybox->setTexture(std::make_shared<Texture>(), 5);

    // Create child nodes
    std::shared_ptr<Pivot> child1 = std::make_shared<Pivot>("child1");
    std::shared_ptr<Node> child2 = std::make_shared<Mesh>("child2");
    std::shared_ptr<Node> child3 = std::make_shared<InstancedMesh>("child3");
    std::shared_ptr<Node> child4 = std::make_shared<AmbientLight>("child4");
    std::shared_ptr<Node> child5 = std::make_shared<PointLight>();
    std::shared_ptr<Node> child6 = std::make_shared<DirectionalLight>();
    std::shared_ptr<Node> child7 = std::make_shared<SpotLight>("child7");

    std::shared_ptr<PerspectiveCamera> child1_1 = std::make_shared<PerspectiveCamera>("child1_1");
    std::shared_ptr<Node> child1_2 = std::make_shared<OrthographicCamera>("child1_2");
    std::shared_ptr<Node> child1_3 = std::make_shared<Node>("child1_3");

    // Add child nodes to the root node
    rootNode->addChild(child1);
    child1->getTransform().translate(glm::vec3(4.0f, 4.0f, 0.0f));
    rootNode->addChild(child2);
    rootNode->addChild(child3);

    child3->addChild(child4);
    child4->addChild(child5);
    child4->addChild(child6);
    child6->addChild(child7);

    child1->addChild(child1_1);
    child1->addChild(child1_2);
    child1->addChild(child1_3);

    rootNode->setCamera(child1_1);

    std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>("skeleton");
    std::shared_ptr<Pivot> spine = std::make_shared<Pivot>("spine");
    std::shared_ptr<Pivot> chest = std::make_shared<Pivot>("chest");
    std::shared_ptr<Pivot> elbow = std::make_shared<Pivot>("elbow");
    std::shared_ptr<Pivot> arm = std::make_shared<Pivot>("arm");
    std::shared_ptr<Pivot> hand = std::make_shared<Pivot>("hand");
    std::shared_ptr<PointLight> torch = std::make_shared<PointLight>("torch");

    rootNode->addChild(skeleton);

    rootNode->addChild(spine);
    spine->addChild(chest);
    chest->addChild(elbow);
    chest->getTransform().translate(glm::vec3(0.0f, 1.0f, 0.0f));
    elbow->addChild(arm);
    elbow->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
    arm->addChild(hand);
    arm->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
    hand->addChild(torch);
    hand->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));

    skeleton->addBone(spine);
    skeleton->addBone(chest);
    skeleton->addBone(elbow);
    skeleton->addBone(arm);
    skeleton->addBone(hand);

    std::shared_ptr<SkinnedMesh> humanMesh = std::make_shared<SkinnedMesh>("human_mesh");

    rootNode->addChild(humanMesh);
    humanMesh->setSkeleton(skeleton);

    rootNode->writeInStream(std::cout);

    glm::mat4 t = hand->getWorldTransformMatrix();
    hand->getTransformMatrixRelativeToNode(elbow);

    std::cout << "t: "
              << std::to_string(t[3][0]) << ","
              << std::to_string(t[3][1]) << ","
              << std::to_string(t[3][2]) << std::endl;
}

void testNodeDynamic()
{
    // Create a root node
    std::shared_ptr<Scene> rootNode = makeNode<Scene>("Scene", "root");

    std::shared_ptr<Skybox> skybox = makeNode<Skybox>("Skybox", "skybox");
    rootNode->addChild(skybox);

    std::shared_ptr<Texture> blank_texture = std::make_shared<Texture>();
    skybox->setTexture(blank_texture, 0);
    // skybox->setTexture(makeNode<Texture>("Texture", ), 5);

    // Create child nodes
    std::shared_ptr<Pivot> child1 = makeNode<Pivot>("Pivot", "child1");
    std::shared_ptr<Node> child2 = makeNode<Mesh>("Mesh", "child2");
    std::shared_ptr<Node> child3 = makeNode<InstancedMesh>("InstancedMesh", "child3");
    std::shared_ptr<Node> child4 = makeNode<AmbientLight>("AmbientLight", "child4");
    std::shared_ptr<Node> child5 = makeNode<PointLight>("PointLight", "point_light");
    std::shared_ptr<Node> child6 = makeNode<DirectionalLight>("DirectionalLight", "directional_light");
    std::shared_ptr<Node> child7 = makeNode<SpotLight>("SpotLight", "child7");

    std::shared_ptr<PerspectiveCamera> child1_1 = makeNode<PerspectiveCamera>("PerspectiveCamera", "child1_1");
    std::shared_ptr<Node> child1_2 = makeNode<OrthographicCamera>("OrthographicCamera", "child1_2");
    std::shared_ptr<Node> child1_3 = makeNode<Node>("Node", "child1_3");

    // Add child nodes to the root node
    rootNode->addChild(child1);
    child1->getTransform().translate(glm::vec3(4.0f, 4.0f, 0.0f));
    rootNode->addChild(child2);
    rootNode->addChild(child3);

    child3->addChild(child4);
    child4->addChild(child5);
    child4->addChild(child6);
    child6->addChild(child7);

    child1->addChild(child1_1);
    child1->addChild(child1_2);
    child1->addChild(child1_3);

    rootNode->setCamera(child1_1);

    std::shared_ptr<Skeleton> skeleton = makeNode<Skeleton>("Skeleton", "skeleton");
    std::shared_ptr<Pivot> spine = makeNode<Pivot>("Pivot", "spine");
    std::shared_ptr<Pivot> chest = makeNode<Pivot>("Pivot", "chest");
    std::shared_ptr<Pivot> elbow = makeNode<Pivot>("Pivot", "elbow");
    std::shared_ptr<Pivot> arm = makeNode<Pivot>("Pivot", "arm");
    std::shared_ptr<Pivot> hand = makeNode<Pivot>("Pivot", "hand");
    std::shared_ptr<PointLight> torch = makeNode<PointLight>("PointLight", "torch");

    rootNode->addChild(skeleton);

    rootNode->addChild(spine);
    spine->addChild(chest);
    chest->addChild(elbow);
    chest->getTransform().translate(glm::vec3(0.0f, 1.0f, 0.0f));
    elbow->addChild(arm);
    elbow->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
    arm->addChild(hand);
    arm->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));
    hand->addChild(torch);
    hand->getTransform().translate(glm::vec3(1.0f, 0.0f, 0.0f));

    skeleton->addBone(spine);
    skeleton->addBone(chest);
    skeleton->addBone(elbow);
    skeleton->addBone(arm);
    skeleton->addBone(hand);

    std::shared_ptr<SkinnedMesh> humanMesh = makeNode<SkinnedMesh>("SkinnedMesh", "human_mesh");

    rootNode->addChild(humanMesh);
    humanMesh->setSkeleton(skeleton);

    rootNode->writeInStream(std::cout);

    glm::mat4 t = hand->getWorldTransformMatrix();

    std::cout << "t: "
              << std::to_string(t[3][0]) << ","
              << std::to_string(t[3][1]) << ","
              << std::to_string(t[3][2]) << std::endl;
}
