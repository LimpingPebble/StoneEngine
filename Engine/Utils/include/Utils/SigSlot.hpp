// Copyright 2024 Stone-Engine

#pragma once

#include <unordered_set>

namespace Stone {

template <typename... Args>
struct Signal;

/**
 * @brief A class template representing a slot in an event system.
 *
 * This class template is used to create slots that can be bound to signals in an event system.
 * Slots are instancied with a callback like functions, lambda expressions, or member functions of a class.
 *
 * Slots can be bind to a (single) signal and will trigger its callback when the signal broadcasts an event.
 *
 * @tparam Args The argument types of the slot's callback function.
 */
template <typename... Args>
struct Slot {

	Slot(const Slot &) = delete;
	Slot &operator=(const Slot &) = delete;

	/**
	 * @brief Constructs a slot with a lambda expression as the callback.
	 *
	 * @param fn A lambda expression representing the callback function.
	 */
	explicit Slot(std::function<void(Args...)> fn) : _fn(std::move(fn)), _signal(nullptr) {
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
	 */
	void perform(Args... args) {
		_fn(std::forward<Args>(args)...);
	}

	/**
	 * @brief Calls the perform function.
	 */
	inline void operator()(Args... args) {
		perform(std::forward<Args>(args)...);
	}

	/**
	 * @brief Unbinds the slot from its signal.
	 *
	 * If the slot is not bound to a signal, this function does nothing.
	 */
	void unbind() {
		if (_signal != nullptr) {
			_signal->unbind(*this);
			_signal = nullptr;
		}
	}

private:
	std::function<void(Args...)> _fn; ///< The callback function of the slot.

	Signal<Args...> *_signal; ///< The signal that the slot is bound to.
	friend Signal<Args...>;
};

/**
 * @brief The Signal class is responsible for managing slots and broadcasting events.
 *
 * A signal can bind multiple slots.
 * When broadcasting a signal, it will trigger all bound slots once.
 *
 * @tparam Args The argument types of the event.
 */
template <typename... Args>
struct Signal {

	Signal(const Signal &) = delete;
	Signal &operator=(const Signal &) = delete;

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
	 * @brief Calls the broadcast function.
	 */
	void operator()(Args... args) {
		broadcast(std::forward<Args>(args)...);
	}

	/**
	 * @brief Binds a slot to the Signal.
	 *        It unbinds the slot from any previous Signal.
	 *
	 * If the slot was already bound to this Signal, nothing will happend.
	 *
	 * @param sub The slot to be bound.
	 */
	void bind(Slot<Args...> &sub) {
		sub.unbind();
		sub._signal = this;
		_slots.insert(&sub);
	}

	/**
	 * @brief Unbinds a slot from this Signal.
	 *
	 * If the slot is not bound to this Signal, nothing will happen.
	 *
	 * @param sub The slot to be unbound.
	 */
	void unbind(Slot<Args...> &sub) {
		if (sub._signal == this) {
			sub._signal = nullptr;
			_slots.erase(&sub);
		}
	}

private:
	std::unordered_set<Slot<Args...> *> _slots; ///< The set of slots bound to this Signal.
};

} // namespace Stone
