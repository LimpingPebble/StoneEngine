// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Delegate.hpp"
#include "Core/Subscriber.hpp"
#include <set>

namespace Stone::Core
{

    template <typename T>
    class Publisher;

    /**
     * @brief The Publisher class is responsible for managing subscribers and broadcasting events.
     *
     * @tparam R The return type of the event.
     * @tparam Args The argument types of the event.
     */
    template <typename R, typename... Args>
    class Publisher<R(Args...)>
    {
    public:
        Publisher(const Publisher &) = delete;
        Publisher &operator=(const Publisher &) = delete;

        /**
         * @brief Default constructor to create a Publisher object.
         */
        Publisher() : _subscribers()
        {
        }

        /**
         * @brief Destructor to clean up the Publisher object.
         *        It removes the Publisher reference from all subscribers.
         */
        virtual ~Publisher()
        {
            for (auto &subscriber : _subscribers)
            {
                if (subscriber != nullptr && subscriber->_publisher == this)
                    subscriber->_publisher = nullptr;
            }
        }

        /**
         * @brief Broadcasts an event to all subscribers.
         *
         * @param args The arguments to be passed to the subscribers.
         */
        void broadcast(Args... args) const
        {
            for (auto &subscriber : _subscribers)
            {
                if (subscriber != nullptr)
                    subscriber->perform(std::forward<Args>(args)...);
            }
        }

        /**
         * @brief Binds a subscriber to the Publisher.
         *        It unbinds the subscriber from any previous Publisher.
         *
         * @param sub The subscriber to be bound.
         */
        void bind(Subscriber<R(Args...)> &sub)
        {
            sub.unbind();
            sub._publisher = this;
            _subscribers.insert(&sub);
        }

        /**
         * @brief Unbinds a subscriber from this Publisher.
         *
         * @param sub The subscriber to be unbound.
         */
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
