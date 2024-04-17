// Copyright 2024 Stone-Engine

#pragma once

#include "Object.hpp"
#include "RenderTypes.hpp"
#include <vector>
#include <functional>

namespace STN
{

    namespace Scene
    {

        class Scene;

        class Node : public Object
        {
        public:
            Node(const std::string &name = "node");
            Node(const Node &other);

            virtual ~Node();

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

            virtual void forEachChild(std::function<void(std::shared_ptr<Node>)> callback, bool recursively = true) const;

            virtual void update(float deltaTime);
            virtual void render(const RenderUniforms &uniforms, RenderStage stage, std::shared_ptr<Scene> scene);

            virtual void writeInStream(std::ostream &flux, std::string linePrefix, std::string firstPrefix, std::string lastPrefix) const;

        protected:
            std::string _name;
            std::vector<std::shared_ptr<Node>> _children;
            std::weak_ptr<Node> _parent;
        };

    } // namespace Scene

} // namespace STN

#define TERM_COLOR_RESET "\033[0m"
#define TERM_COLOR_BLACK "\033[30m"
#define TERM_COLOR_RED "\033[31m"
#define TERM_COLOR_GREEN "\033[32m"
#define TERM_COLOR_YELLOW "\033[33m"
#define TERM_COLOR_BLUE "\033[34m"
#define TERM_COLOR_MAGENTA "\033[35m"
#define TERM_COLOR_CYAN "\033[36m"
#define TERM_COLOR_WHITE "\033[37m"
#define TERM_COLOR_BOLDBLACK "\033[1m\033[30m"
#define TERM_COLOR_BOLDRED "\033[1m\033[31m"
#define TERM_COLOR_BOLDGREEN "\033[1m\033[32m"
#define TERM_COLOR_BOLDYELLOW "\033[1m\033[33m"
#define TERM_COLOR_BOLDBLUE "\033[1m\033[34m"
#define TERM_COLOR_BOLDMAGENTA "\033[1m\033[35m"
#define TERM_COLOR_BOLDCYAN "\033[1m\033[36m"
#define TERM_COLOR_BOLDWHITE "\033[1m\033[37m"
