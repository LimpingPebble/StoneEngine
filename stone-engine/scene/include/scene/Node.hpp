// Copyright 2024 Stone-Engine

#pragma once

#include "core/Object.hpp"
#include "scene/RenderContext.hpp"
#include "logging/TermColor.hpp"
#include <vector>
#include <functional>

namespace Stone
{

    namespace Scene
    {

        class Node : public Core::Object
        {
        public:
            Node(const std::string &name = "node");
            Node(const Node &other);

            virtual ~Node();

            virtual const char *getClassName() const override;
            virtual std::string debugDescription() const;

            void setName(const std::string &name);
            const std::string &getName() const;
            std::string getGlobalName() const;

            void addChild(std::shared_ptr<Node> child);
            void removeChild(std::shared_ptr<Node> child);
            void removeFromParent();

            std::shared_ptr<Node> getParent() const;
            bool hasParent() const;
            bool isAncestorOf(std::shared_ptr<Node> node) const;
            bool isDescendantOf(std::shared_ptr<Node> node) const;
            const std::vector<std::shared_ptr<Node>> &getChildren() const;

            std::shared_ptr<Node> getChild(const std::string &name) const;
            /**
             * @brief Get a child node with a path
             *
             * @param path The path to the child node. If starting with '*\/', then it will look for any descendant with the given name
             *
             * @example getChildWithPath("child1/child2/child3")
             * @example getChildWithPath("*\/child3")
             *
             * @return `std::shared_ptr<Node>` The child node
             */
            std::shared_ptr<Node> getChildWithPath(const std::string &path) const;

            template <typename T>
            std::shared_ptr<T> getChild(const std::string &name) const
            {
                return std::dynamic_pointer_cast<T>(getChild(name));
            }

            template <typename T>
            std::shared_ptr<T> getChildWithPath(const std::string &path) const
            {
                return std::dynamic_pointer_cast<T>(getChildWithPath(path));
            }

            virtual void transformRelativeMatrix(glm::mat4 &relative) const;
            const glm::mat4 getWorldTransformMatrix() const;
            const glm::mat4 getTransformMatrixRelativeToNode(std::shared_ptr<Node> otherNode) const;

            void withAllChildrenHierarchy(std::function<void(std::shared_ptr<Node>)> callback) const;

            virtual void update(float deltaTime);
            virtual void render(RenderContext &context);

            void writeInStream(std::ostream &flux, std::string linePrefix = "", std::string firstPrefix = "", std::string lastPrefix = "", bool colored = true) const;

        protected:
            std::string _name;
            std::vector<std::shared_ptr<Node>> _children;
            std::weak_ptr<Node> _parent;

            virtual const char *_termClassColor() const;
        };

    } // namespace Scene

} // namespace Stone
