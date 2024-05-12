// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Transform.hpp"
#include "Utils/CurveFunction.hpp"

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
	Interpolator(const Interpolator &copy) = default;

	virtual ~Interpolator() = default;

	Interpolator &operator=(const Interpolator &copy) = default;

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

/**
 * @brief A class that interpolates 2D transformations over time.
 *
 * This class provides interpolation functionality for 2D transformations, including position, rotation, and scale.
 * It uses an Interpolator class to store and interpolate key values at different times.
 * The class supports adding key values at specific times and retrieving interpolated transformations at a given time.
 * It also provides a method to calculate the duration of the interpolation based on the durations of the individual key
 * value interpolators.
 *
 * @tparam Time The type of time used for interpolation (default: float).
 */
template <typename Time = float>
class Transform2DInterpolator {
public:
	Transform2DInterpolator() = default;
	Transform2DInterpolator(const Transform2DInterpolator &copy) = default;

	virtual ~Transform2DInterpolator() = default;

	Transform2DInterpolator &operator=(const Transform2DInterpolator &copy) = default;

	/**
	 * @brief Get a const reference to the position key value interpolator.
	 *
	 * @return const Interpolator<glm::vec2>& A const reference to the position key value interpolator.
	 */
	const Interpolator<glm::vec2> &getKeyPositions() const {
		return _keyPositions;
	}

	/**
	 * @brief Get a reference to the position key value interpolator.
	 *
	 * @return Interpolator<glm::vec2>& A reference to the position key value interpolator.
	 */
	Interpolator<glm::vec2> &keyPositionsRef() {
		return _keyPositions;
	}

	/**
	 * @brief Get a const reference to the rotation key value interpolator.
	 *
	 * @return const Interpolator<float>& A const reference to the rotation key value interpolator.
	 */
	const Interpolator<float> &getKeyRotations() const {
		return _keyRotations;
	}

	/**
	 * @brief Get a reference to the rotation key value interpolator.
	 *
	 * @return Interpolator<float>& A reference to the rotation key value interpolator.
	 */
	Interpolator<float> &keyRotationsRef() {
		return _keyRotations;
	}

	/**
	 * @brief Get a const reference to the scale key value interpolator.
	 *
	 * @return const Interpolator<glm::vec2>& A const reference to the scale key value interpolator.
	 */
	const Interpolator<glm::vec2> &getKeyScales() const {
		return _keyScales;
	}

	/**
	 * @brief Get a reference to the scale key value interpolator.
	 *
	 * @return Interpolator<glm::vec2>& A reference to the scale key value interpolator.
	 */
	Interpolator<glm::vec2> &keyScalesRef() {
		return _keyScales;
	}

	/**
	 * @brief Add a position key value at a specific time.
	 *
	 * @param time The time at which to add the position key value.
	 * @param position The position key value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addPositionAt(Time time, glm::vec2 position, Curve::Function<Time> function = Curve::Function<Time>()) {
		_keyPositions.addKeyValueAt(position, time, std::move(function));
	}

	/**
	 * @brief Add a rotation key value at a specific time.
	 *
	 * @param time The time at which to add the rotation key value.
	 * @param rotation The rotation key value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addRotationAt(Time time, float rotation, Curve::Function<Time> function = Curve::Function<Time>()) {
		_keyRotations.addKeyValueAt(rotation, time, std::move(function));
	}

	/**
	 * @brief Add a scale key value at a specific time.
	 *
	 * @param time The time at which to add the scale key value.
	 * @param scale The scale key value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addScaleAt(Time time, glm::vec2 scale, Curve::Function<Time> function = Curve::Function<Time>()) {
		_keyScales.addKeyValueAt(scale, time, std::move(function));
	}

	/**
	 * @brief Add a matrix key value at a specific time.
	 *
	 * This method decomposes the matrix into position, rotation, and scale components and adds them as key values.
	 *
	 * @param time The time at which to add the matrix key value.
	 * @param matrix The matrix key value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addMatrixAt(Time time, glm::mat4 matrix, Curve::Function<Time> function = Curve::Function<Time>()) {
		Transform2D t;
		t.setMatrix(matrix);
		addPositionAt(time, t.getPosition(), std::move(function));
		addRotationAt(time, t.getRotation(), std::move(function));
		addScaleAt(time, t.getScale(), std::move(function));
	}

	/**
	 * @brief Get the interpolated transformation at a specific time.
	 *
	 * This method retrieves the interpolated position, rotation, and scale values at the given time and applies them to
	 * the provided default transformation.
	 *
	 * @param time The time at which to retrieve the interpolated transformation.
	 * @param __default The default transformation to apply the interpolated values to (default: Transform2D()).
	 * @return Transform2D The interpolated transformation.
	 */
	Transform2D transformAt(Time time, Transform2D __default = Transform2D()) const {
		if (!_keyPositions.isEmpty()) {
			__default.setPosition(_keyPositions.valueAt(time));
		}
		if (!_keyRotations.isEmpty()) {
			__default.setRotation(_keyRotations.valueAt(time));
		}
		if (!_keyScales.isEmpty()) {
			__default.setScale(_keyScales.valueAt(time));
		}
		return __default;
	}

	/**
	 * @brief Get the duration of the interpolation.
	 *
	 * This method calculates the duration of the interpolation based on the durations of the individual key value
	 * interpolators.
	 *
	 * @return Time The duration of the interpolation.
	 */
	Time duration() const {
		return std::max({_keyPositions.duration(), _keyRotations.duration(), _keyScales.duration()});
	}

private:
	Interpolator<glm::vec2> _keyPositions; /**< Interpolator for position key values. */
	Interpolator<float> _keyRotations;	   /**< Interpolator for rotation key values. */
	Interpolator<glm::vec2> _keyScales;	   /**< Interpolator for scale key values. */
};

/**
 * @brief A class that interpolates 3D transformations over time.
 *
 * This class provides interpolation functionality for key positions, rotations, and scales
 * in a 3D space. It uses an Interpolator class to store and interpolate the key values.
 *
 * @tparam Time The type of time used for interpolation (default: float).
 */
template <typename Time = float>
class Transform3DInterpolator {
public:
	Transform3DInterpolator() = default;
	Transform3DInterpolator(const Transform3DInterpolator &copy) = default;

	virtual ~Transform3DInterpolator() = default;

	Transform3DInterpolator &operator=(const Transform3DInterpolator &copy) = default;

	/**
	 * @brief Get the const reference to the key positions interpolator.
	 *
	 * @return const Interpolator<glm::vec3>& The const reference to the key positions interpolator.
	 */
	const Interpolator<glm::vec3> &getKeyPositions() const {
		return _keyPositions;
	}

	/**
	 * @brief Get the reference to the key positions interpolator.
	 *
	 * @return Interpolator<glm::vec3>& The reference to the key positions interpolator.
	 */
	Interpolator<glm::vec3> &keyPositionsRef() {
		return _keyPositions;
	}

	/**
	 * @brief Get the const reference to the key rotations interpolator.
	 *
	 * @return const Interpolator<glm::quat>& The const reference to the key rotations interpolator.
	 */
	const Interpolator<glm::quat> &getKeyRotations() const {
		return _keyRotations;
	}

	/**
	 * @brief Get the reference to the key rotations interpolator.
	 *
	 * @return Interpolator<glm::quat>& The reference to the key rotations interpolator.
	 */
	Interpolator<glm::quat> &keyRotationsRef() {
		return _keyRotations;
	}

	/**
	 * @brief Get the const reference to the key scales interpolator.
	 *
	 * @return const Interpolator<glm::vec3>& The const reference to the key scales interpolator.
	 */
	const Interpolator<glm::vec3> &getKeyScales() const {
		return _keyScales;
	}

	/**
	 * @brief Get the reference to the key scales interpolator.
	 *
	 * @return Interpolator<glm::vec3>& The reference to the key scales interpolator.
	 */
	Interpolator<glm::vec3> &keyScalesRef() {
		return _keyScales;
	}

	/**
	 * @brief Add a key position at a specific time.
	 *
	 * @param time The time at which to add the key position.
	 * @param position The position value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addPositionAt(Time time, glm::vec3 position, Curve::Function<Time> function = Curve::Function<Time>()) {
		_keyPositions.addKeyValueAt(position, time, function);
	}

	/**
	 * @brief Add a key rotation at a specific time.
	 *
	 * @param time The time at which to add the key rotation.
	 * @param rotation The rotation value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addRotationAt(Time time, glm::quat rotation, Curve::Function<Time> function = Curve::Function<Time>()) {
		_keyRotations.addKeyValueAt(rotation, time, function);
	}

	/**
	 * @brief Add a key scale at a specific time.
	 *
	 * @param time The time at which to add the key scale.
	 * @param scale The scale value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addScaleAt(Time time, glm::vec3 scale, Curve::Function<Time> function = Curve::Function<Time>()) {
		_keyScales.addKeyValueAt(scale, time, function);
	}

	/**
	 * @brief Add a key matrix at a specific time.
	 *
	 * This method decomposes the matrix into position, rotation, and scale components
	 * and adds them as key values.
	 *
	 * @param time The time at which to add the key matrix.
	 * @param matrix The matrix value to add.
	 * @param function The interpolation function to use (default: Curve::Function<Time>()).
	 */
	void addMatrixAt(Time time, glm::mat4 matrix, Curve::Function<Time> function = Curve::Function<Time>()) {
		Transform3D t;
		t.setMatrix(matrix);
		addPositionAt(time, t.getPosition(), std::move(function));
		addRotationAt(time, t.getRotation(), std::move(function));
		addScaleAt(time, t.getScale(), std::move(function));
	}

	/**
	 * @brief Get the interpolated transformation at a specific time.
	 *
	 * This method interpolates the key positions, rotations, and scales at the given time
	 * and returns the resulting Transform3D object.
	 *
	 * @param time The time at which to get the interpolated transformation.
	 * @param __default The default Transform3D object to use if no key values are available (default: Transform3D()).
	 * @return Transform3D The interpolated transformation.
	 */
	Transform3D transformAt(Time time, Transform3D __default = Transform3D()) const {
		if (!_keyPositions.isEmpty()) {
			__default.setPosition(_keyPositions.valueAt(time));
		}
		if (!_keyRotations.isEmpty()) {
			__default.setRotation(_keyRotations.valueAt(time));
		}
		if (!_keyScales.isEmpty()) {
			__default.setScale(_keyScales.valueAt(time));
		}
		return __default;
	}

	/**
	 * @brief Get the duration of the interpolator.
	 *
	 * This method returns the maximum duration among the key positions, rotations, and scales.
	 *
	 * @return Time The duration of the interpolator.
	 */
	Time duration() const {
		return std::max({_keyPositions.duration(), _keyRotations.duration(), _keyScales.duration()});
	}

private:
	Interpolator<glm::vec3> _keyPositions; /**< The interpolator for key positions. */
	Interpolator<glm::quat> _keyRotations; /**< The interpolator for key rotations. */
	Interpolator<glm::vec3> _keyScales;	   /**< The interpolator for key scales. */
};

} // namespace Stone::Scene
