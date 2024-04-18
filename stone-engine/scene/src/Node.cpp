// Copyright 2024 Stone-Engine

#include "Node.hpp"
#include "Pivot.hpp"
#include <cassert>

namespace STN
{

    namespace Scene
    {

        Node::Node(const std::string &name)
            : Object(), _name(name), _children(), _parent()
        {
            // LOG: Warning: Node name cannot contain '/'
            assert(name.find('/') == std::string::npos);
        }

        Node::Node(const Node &other)
            : Object(other), _name(other._name), _children(other._children), _parent(other._parent)
        {
        }

        Node::~Node()
        {
        }

        const char *Node::getClassName() const
        {
            return "Node";
        }

        std::string Node::debugDescription() const
        {
            std::string str = Object::debugDescription();
            str.pop_back();
            str += ",name:\"" + _name + "\"}";
            return str;
        }

        void Node::setName(const std::string &name)
        {
            _name = name;
        }

        const std::string &Node::getName() const
        {
            return _name;
        }

        std::string Node::getGlobalName() const
        {
            if (auto parent = getParent())
            {
                return parent->getGlobalName() + "/" + getName();
            }
            return "/" + getName();
        }

        void Node::addChild(std::shared_ptr<Node> child)
        {
            // LOG: Error: Cannot add a parent as a child
            assert(!child->isAncestorOf(std::dynamic_pointer_cast<Node>(shared_from_this())));
            child->_parent = std::dynamic_pointer_cast<Node>(shared_from_this());
            _children.push_back(child);
        }

        void Node::removeChild(std::shared_ptr<Node> child)
        {
            auto it = std::find(_children.begin(), _children.end(), child);
            if (it != _children.end())
            {
                (*it)->_parent.reset();
                _children.erase(it);
            }
        }

        void Node::removeFromParent()
        {
            if (auto parent = getParent())
            {
                parent->removeChild(std::dynamic_pointer_cast<Node>(shared_from_this()));
            }
        }

        std::shared_ptr<Node> Node::getParent() const
        {
            return _parent.lock();
        }

        bool Node::hasParent() const
        {
            return !_parent.expired();
        }

        bool Node::isAncestorOf(std::shared_ptr<Node> node) const
        {
            if (auto parent = node->getParent())
            {
                if (parent.get() == this)
                {
                    return true;
                }
                return isAncestorOf(parent);
            }
            return false;
        }

        bool Node::isDescendantOf(std::shared_ptr<Node> node) const
        {
            if (auto parent = getParent())
            {
                if (parent == node)
                {
                    return true;
                }
                return parent->isDescendantOf(node);
            }
            return false;
        }

        const std::vector<std::shared_ptr<Node>> &Node::getChildren() const
        {
            return _children;
        }

        std::shared_ptr<Node> Node::getChild(const std::string &name) const
        {
            for (auto &child : _children)
            {
                if (child->getName() == name)
                {
                    return child;
                }
            }
            return nullptr;
        }

        std::shared_ptr<Node> Node::getChildWithPath(const std::string &path) const
        {
            if (path.size() <= 1)
            {
                return getChild(path);
            }

            if (path[0] == '*' && path[1] == '/')
            {
                std::string childName = path.substr(2);
                for (auto &child : _children)
                {
                    if (child->getName() == childName)
                    {
                        return child;
                    }
                    std::shared_ptr<Node> grandChild = child->getChildWithPath(path);
                    if (grandChild)
                    {
                        return grandChild;
                    }
                }
                return nullptr;
            }

            auto firstSlash = path.find('/');

            if (firstSlash != std::string::npos)
            {
                std::string childName = path.substr(0, firstSlash);
                auto childNode = getChild(childName);
                if (!childNode)
                {
                    return nullptr;
                }
                std::string subpath = path.substr(firstSlash + 1);
                return childNode->getChildWithPath(subpath);
            }

            return getChild(path);
        }

        void Node::transformRelativeMatrix(glm::mat4& relative) const
        {
            (void)relative;
        }

        const glm::mat4 Node::getWorldTransformMatrix() const
        {
            return getTransformMatrixRelativeToNode(nullptr);
        }

        const glm::mat4 Node::getTransformMatrixRelativeToNode(std::shared_ptr<Node> otherNode) const
        {
            glm::mat4 transform(1);
            if (this == otherNode.get())
            {
                return transform;
            }

            transformRelativeMatrix(transform);

            std::shared_ptr<Node> node = getParent();
            while (true) {
                if (node == nullptr)
                {
                    if (!otherNode)
                        break;

                    return glm::inverse(otherNode->getWorldTransformMatrix()) * transform;
                }
                if (node == otherNode)
                {
                    break;
                }
                node->transformRelativeMatrix(transform);
                node = node->getParent();
            }
            return transform;
        }

        void Node::withAllChildrenHierarchy(std::function<void(std::shared_ptr<Node>)> callback) const
        {
            for (auto &child : _children)
            {
                child->withAllChildrenHierarchy(callback);
                callback(child);
            }
        }

        void Node::update(float deltaTime)
        {
            for (auto &child : _children)
            {
                child->update(deltaTime);
            }
        }

        void Node::render(RenderContext &context)
        {
            for (auto &child : _children)
            {
                child->render(context);
            }
        }

        void Node::writeInStream(std::ostream &stream, std::string linePrefix, std::string firstPrefix, std::string lastPrefix, bool colored) const
        {
            stream << linePrefix << firstPrefix;
            if (colored)
            {
                stream << TERM_COLOR_BOLD TERM_COLOR_WHITE << _name << TERM_COLOR_RESET;
                stream << " [" << _termClassColor() << getClassName() << TERM_COLOR_RESET << "] ";
            }
            else
            {
                stream << _name << " [" << getClassName() << "] ";
            }
            stream << debugDescription() << std::endl;
            for (size_t i = 0; i < _children.size(); i++)
            {
                if (i == _children.size() - 1)
                {
                    _children[i]->writeInStream(stream, linePrefix + lastPrefix, "└─", "  ", colored);
                }
                else
                {
                    _children[i]->writeInStream(stream, linePrefix + lastPrefix, "├─", "│ ", colored);
                }
            }
        }

        const char *Node::_termClassColor() const
        {
            return TERM_COLOR_BOLD TERM_COLOR_GRAY;
        }

    } // namespace Scene

} // namespace STN
