// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Logging/TermColor.hpp"
#include "Scene/Node/NodeMacros.hpp"
#include "Scene/RenderContext.hpp"

#include <functional>
#include <vector>

namespace Stone::Scene {

class WorldNode;

class Node : public Core::Object {
public:
	STONE_NODE(Node);

	explicit Node(const std::string &name = "node");
	Node(const Node &other) = default;

	~Node() override = default;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	virtual void update(float deltaTime);
	virtual void render(RenderContext &context);

	void setName(const std::string &name);
	[[nodiscard]] const std::string &getName() const;
	[[nodiscard]] std::string getGlobalName() const;

	void addChild(const std::shared_ptr<Node> &child);

	template <typename T>
	std::shared_ptr<T> addChild(const std::string &name = T::className) {
		auto child = std::make_shared<T>(name);
		addChild(child);
		return child;
	}

	void removeChild(const std::shared_ptr<Node> &child);
	void removeFromParent();

	[[nodiscard]] std::shared_ptr<Node> getParent() const;
	[[nodiscard]] bool hasParent() const;
	[[nodiscard]] bool isAncestorOf(const std::shared_ptr<Node> &node) const;
	[[nodiscard]] bool isDescendantOf(const std::shared_ptr<Node> &node) const;

	const std::vector<std::shared_ptr<Node>> &getChildren() const;

	[[nodiscard]] std::shared_ptr<Node> getChild(const std::string &name) const;
	/**
	 * @brief Get a child node with a path
	 *
	 * @param path The path to the child node. If starting with '*\/', then it will look for any descendant with the
	 * given name
	 *
	 * @example getChildByPath("child1/child2/child3")
	 * @example getChildByPath("*\/child3")
	 *
	 * @return `std::shared_ptr<Node>` The child node
	 */
	[[nodiscard]] std::shared_ptr<Node> getChildByPath(const std::string &path) const;

	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getChild(const std::string &name) const {
		return std::dynamic_pointer_cast<T>(getChild(name));
	}

	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getChildByPath(const std::string &path) const {
		return std::dynamic_pointer_cast<T>(getChildByPath(path));
	}

	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getChildByClass() const {
		for (auto &child : _children) {
			std::shared_ptr<T> node = std::dynamic_pointer_cast<T>(child);
			if (node)
				return node;
		}
		return nullptr;
	}

	[[nodiscard]] std::shared_ptr<WorldNode> getWorld() const;

	virtual void transformRelativeMatrix(glm::mat4 &relative) const;
	[[nodiscard]] glm::mat4 getWorldTransformMatrix() const;
	[[nodiscard]] glm::mat4 getTransformMatrixRelativeToNode(const std::shared_ptr<Node> &otherNode) const;

	void withAllChildrenHierarchy(const std::function<void(const std::shared_ptr<Node> &)> &callback) const;

	void writeHierarchy(std::ostream &stream, bool colored = true, const std::string &linePrefix = "",
						const std::string &firstPrefix = "", const std::string &lastPrefix = "") const;

protected:
	std::string _name;
	std::vector<std::shared_ptr<Node>> _children;
	std::weak_ptr<Node> _parent;
	std::weak_ptr<WorldNode> _world;

	[[nodiscard]] virtual const char *_termClassColor() const;
};

} // namespace Stone::Scene
