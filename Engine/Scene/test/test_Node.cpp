
#include "Scene/Node/Node.hpp"

#include <gtest/gtest.h>

using namespace Stone::Scene;

TEST(Node, GetGlobalName) {
	// Create nodes
	auto root = std::make_shared<Node>("Root");
	auto child1 = std::make_shared<Node>("Child1");
	auto child2 = std::make_shared<Node>("Child2");
	auto grandchild = std::make_shared<Node>("Grandchild");

	// Build hierarchy
	root->addChild(child1);
	root->addChild(child2);
	child2->addChild(grandchild);

	// Check global names
	EXPECT_EQ(root->getGlobalName(), "/Root");
	EXPECT_EQ(child1->getGlobalName(), "/Root/Child1");
	EXPECT_EQ(child2->getGlobalName(), "/Root/Child2");
	EXPECT_EQ(grandchild->getGlobalName(), "/Root/Child2/Grandchild");
}
