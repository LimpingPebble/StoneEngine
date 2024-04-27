// Copyright 2024 Stone-Engine

#include "Utils/Glm.hpp"

#include <glm/ext.hpp>

namespace std {
string to_string(const glm::vec2 &v) {
	return "{x:" + std::to_string(v.x) + ",y:" + std::to_string(v.y) + "}";
}

string to_string(const glm::vec3 &v, bool color) {
	if (color)
		return "{r:" + std::to_string(v.r) + ",g:" + std::to_string(v.g) + ",b:" + std::to_string(v.b) + "}";
	return "{x:" + std::to_string(v.x) + ",y:" + std::to_string(v.y) + ",z:" + std::to_string(v.z) + "}";
}

string to_string(const glm::vec4 &v, bool color) {
	if (color)
		return "{r:" + std::to_string(v.r) + ",g:" + std::to_string(v.g) + ",b:" + std::to_string(v.b) +
			   ",a:" + std::to_string(v.a) + "}";
	return "{x:" + std::to_string(v.x) + ",y:" + std::to_string(v.y) + ",z:" + std::to_string(v.z) +
		   ",w:" + std::to_string(v.w) + "}";
}

string to_string(const glm::mat3 &m) {
	string str = "{";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			str += std::to_string(m[i][j]);
			if (j != 2) {
				str += ",";
			}
		}
		if (i != 2) {
			str += ",";
		}
	}
	str += "}";
	return str;
}

string to_string(const glm::mat4 &m) {
	string str = "{";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			str += std::to_string(m[i][j]);
			if (j != 3) {
				str += ",";
			}
		}
		if (i != 3) {
			str += ",";
		}
	}
	str += "}";
	return str;
}

string to_string(const glm::quat &q) {
	return "{x:" + std::to_string(q.x) + ",y:" + std::to_string(q.y) + ",z:" + std::to_string(q.z) +
		   ",w:" + std::to_string(q.w) + "}";
}
} // namespace std

namespace glm {

float &value_at(mat3 &m, int r, int c) {
	return value_ptr(m)[3 * c + r];
}

float &value_at(mat4 &m, int r, int c) {
	return value_ptr(m)[4 * c + r];
}

} // namespace glm
