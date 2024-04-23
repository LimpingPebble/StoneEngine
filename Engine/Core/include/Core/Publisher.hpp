// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Delegate.hpp"
#include "Core/Subscriber.hpp"
#include <set>

namespace Stone::Core
{

    template <typename T>
    class Publisher;

    template <typename R, typename... Args>
    class Publisher<R(Args...)>
    {
    public:
        Publisher(const Publisher &) = delete;
        Publisher &operator=(const Publisher &) = delete;

        Publisher() : _subscribers()
        {
        }

        ~Publisher()
        {
            for (auto &subscriber : _subscribers)
            {
                if (subscriber != nullptr && subscriber->_publisher == this)
                    subscriber->_publisher = nullptr;
            }
        }

        void broadcast(Args... args) const
        {
            for (auto &subscriber : _subscribers)
            {
                if (subscriber != nullptr)
                    subscriber->perform(std::forward<Args>(args)...);
            }
        }

        void bind(Subscriber<R(Args...)> &sub)
        {
            sub.unbind();
            sub._publisher = this;
            _subscribers.insert(&sub);
        }

        void unbind(Subscriber<R(Args...)> &sub)
        {
            if (sub._publisher == this)
            {
                sub._publisher = nullptr;
                _subscribers.erase(&sub);
            }
        }

    private:
        std::set<Subscriber<R(Args...)> *> _subscribers;
    };

} // namespace Stone::Core
