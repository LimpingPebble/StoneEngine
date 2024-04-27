// Copyright 2024 Stone-Engine

#pragma once

#include "Utils/Delegate.hpp"

#include <set>

namespace Stone {

template <typename T>
class Signal;

template <typename T>
class Slot;

/**
 * @brief A class template representing a slot in an event system.
 *
 * This class template is used to create slots that can be bound to signals in an event system.
 * Slots can be bound to functions, lambda expressions, or member functions of a class.
 *
 * @tparam R The return type of the slot's callback function.
 * @tparam Args The argument types of the slot's callback function.
 */
template <typename R, typename... Args>
class Slot<R(Args...)> {
public:
	Slot(const Slot &) = delete;
	Slot &operator=(const Slot &) = delete;

	/**
	 * @brief Constructs a slot with a function pointer as the callback.
	 *
	 * @param fn A function pointer to the callback function.
	 */
	Slot(R (*fn)(Args... args)) : _signal(nullptr) {
		new (_delegateptr) FunctionDelegate<R(Args...)>(fn);
	}

	/**
	 * @brief Constructs a slot with a lambda expression as the callback.
	 *
	 * @param fn A lambda expression representing the callback function.
	 */
	Slot(std::function<R(Args...)> fn) : _signal(nullptr) {
		new (_delegateptr) LambdaDelegate<R(Args...)>(fn);
	}

	/**
	 * @brief Constructs a slot with a member function pointer as the callback.
	 *
	 * @tparam C The class type that the member function belongs to.
	 * @param target A pointer to an instance of the class.
	 * @param method A pointer to the member function.
	 */
	template <typename C>
	Slot(C *target, R (C::*method)(Args... args)) : _signal(nullptr) {
		new (_delegateptr) MethodDelegate<R (C::*)(Args...)>(target, method);
	}

	/**
	 * @brief Constructs a slot with a const member function pointer as the callback.
	 *
	 * @tparam C The class type that the member function belongs to.
	 * @param target A pointer to a const instance of the class.
	 * @param method A pointer to the const member function.
	 */
	template <typename C>
	Slot(const C *target, R (C::*method)(Args... args) const) : _signal(nullptr) {
		new (_delegateptr) ConstMethodDelegate<R (C::*)(Args...)>(target, method);
	}

	/**
	 * @brief Destructor.
	 *
	 * Unbinds the slot from the signal.
	 */
	virtual ~Slot() {
		unbind();
	}

	/**
	 * @brief Calls the callback function of the slot.
	 *
	 * @param args The arguments to be passed to the callback function.
	 * @return The return value of the callback function.
	 */
	R perform(Args... args) {
		return reinterpret_cast<IDelegate<R(Args...)> *>(_delegateptr)->perform(std::forward<Args>(args)...);
	}

	/**
	 * @brief Unbinds the slot from the signal.
	 *
	 * If the slot is bound to a signal, it will be unbound.
	 */
	void unbind() {
		if (_signal != nullptr) {
			_signal->unbind(*this);
			_signal = nullptr;
		}
	}

private:
	uint8_t _delegateptr[sizeof(LambdaDelegate<void()>)];

	Signal<R(Args...)> *_signal;
	friend Signal<R(Args...)>;
};

template <typename T>
class Slot;

/**
 * @brief The Signal class is responsible for managing slots and broadcasting events.
 *
 * @tparam R The return type of the event.
 * @tparam Args The argument types of the event.
 */
template <typename R, typename... Args>
class Signal<R(Args...)> {
public:
	Signal(const Signal &) = delete;
	Signal &operator=(const Signal &) = delete;

	/**
	 * @brief Default constructor to create a Signal object.
	 */
	Signal() = default;

	/**
	 * @brief Destructor to clean up the Signal object.
	 *        It removes the Signal reference from all slots.
	 */
	virtual ~Signal() {
		for (auto &slot : _slots) {
			if (slot != nullptr && slot->_signal == this)
				slot->_signal = nullptr;
		}
	}

	/**
	 * @brief Broadcasts an event to all slots.
	 *
	 * @param args The arguments to be passed to the slots.
	 */
	void broadcast(Args... args) const {
		for (auto &slot : _slots) {
			if (slot != nullptr)
				slot->perform(std::forward<Args>(args)...);
		}
	}

	/**
	 * @brief Binds a slot to the Signal.
	 *        It unbinds the slot from any previous Signal.
	 *
	 * @param sub The slot to be bound.
	 */
	void bind(Slot<R(Args...)> &sub) {
		sub.unbind();
		sub._signal = this;
		_slots.insert(&sub);
	}

	/**
	 * @brief Unbinds a slot from this Signal.
	 *
	 * @param sub The slot to be unbound.
	 */
	void unbind(Slot<R(Args...)> &sub) {
		if (sub._signal == this) {
			sub._signal = nullptr;
			_slots.erase(&sub);
		}
	}

private:
	std::set<Slot<R(Args...)> *> _slots;
};

} // namespace Stone
