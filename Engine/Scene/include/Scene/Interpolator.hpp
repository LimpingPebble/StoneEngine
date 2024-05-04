// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Transform.hpp"
#include "Utils/CurveFunction.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>

namespace Stone::Scene {

/**
 * @brief A class template for interpolating values over time.
 *
 * The Interpolator class allows you to interpolate between key values over a specified duration.
 * It supports adding and removing key values at specific times, as well as retrieving the interpolated value at a given
 * time.
 *
 * @tparam KeyValue The type of the values to be interpolated.
 * @tparam Time The type representing time. Default is float.
 */
template <typename KeyValue, typename Time = float>
class Interpolator {
public:
	Interpolator() = default;
	Interpolator(const Interpolator<KeyValue> &copy) = default;

	virtual ~Interpolator() = default;

	Interpolator &operator=(const Interpolator<KeyValue> &copy) = default;

	/**
	 * @brief Check if the Interpolator does not contains any key values.
	 * @return True if the Interpolator is empty, false otherwise.
	 */
	bool isEmpty() const {
		return _keyValues.empty();
	}

	/**
	 * @brief Get the duration of the Interpolator.
	 * @return The duration of the Interpolator.
	 */
	Time duration() const {
		if (_keyValues.empty()) {
			return 0;
		} else {
			return _keyValues.rbegin()->first;
		}
	}

	/**
	 * @brief Add a key value at a specific time.
	 * @param value The key value to be added.
	 * @param time The time at which the key value should be added.
	 * @param function The interpolation function to be used. Default is linear interpolation.
	 */
	void addKeyValueAt(const KeyValue &value, Time time,
					   Curve::Function<Time> function = Curve::Function<Time>::linear()) {
		_keyValues[time] = Value(value, function);
	}

	/**
	 * @brief Remove a key value at a specific index.
	 * @param index The index of the key value to be removed.
	 */
	void removeKeyValueAt(int index) {
		auto it = _keyValues.begin();
		std::advance(it, index);
		if (it != _keyValues.end()) {
			_keyValues.erase(it);
		}
	}

	/**
	 * @brief Remove a key value at a specific time.
	 * @param time The time at which the key value should be removed.
	 */
	void removeKeyValueAt(Time time) {
		auto it = _keyValues.lower_bound(time);
		if (it != _keyValues.end()) {
			_keyValues.erase(it);
		}
	}

	/**
	 * @brief Get the interpolated value at a specific time.
	 * @param time The time at which to retrieve the interpolated value.
	 * @return The interpolated value at the specified time.
	 */
	KeyValue valueAt(Time time) const {
		if (_keyValues.empty())
			return KeyValue();

		auto next = _keyValues.lower_bound(time);
		if (next == _keyValues.begin())
			return next->second.value;

		auto prev = std::prev(next);
		if (next == _keyValues.end())
			return prev->second.value;

		Time t = (time - prev->first) / (next->first - prev->first);
		t = next->second.function(t);
		return glm::mix(prev->second.value, next->second.value, t);
	}

private:
	struct Value {
		KeyValue value;					///< The key value.
		Curve::Function<Time> function; ///< The interpolation function.

		Value() = default;
		Value(KeyValue newValue, Curve::Function<Time> newFunction) : value(newValue), function(newFunction) {
		}
	};

	std::map<Time, Value> _keyValues; ///< The map of key values and their associated times.
};

} // namespace Stone::Scene
