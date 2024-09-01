// Copyright 2024 Stone-Engine

#include "Utils/StringExt.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>


bool string_ends_with(const std::string &str, const std::string &suffix) {
	if (str.size() < suffix.size()) {
		return false;
	}
	return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool string_starts_with(const std::string &str, const std::string &prefix) {
	if (str.size() < prefix.size()) {
		return false;
	}
	return str.compare(0, prefix.size(), prefix) == 0;
}

bool string_contains_non_printable(const std::string &str) {
	return std::any_of(str.begin(), str.end(), [](char c) {
		return !std::isprint(static_cast<unsigned char>(c)) && !std::isspace(static_cast<unsigned char>(c));
	});
}

std::string int_to_hex(unsigned char c) {
	std::stringstream ss;
	ss << std::hex << std::uppercase;
	ss << std::setw(4) << std::setfill('0') << static_cast<int>(c);
	return ss.str();
}

std::string escape_string(const std::string &str) {
	std::string result;
	for (char c : str) {
		switch (c) {
		case '\"': result += "\\\""; break;
		case '\\': result += "\\\\"; break;
		case '\b': result += "\\b"; break;
		case '\f': result += "\\f"; break;
		case '\n': result += "\\n"; break;
		case '\r': result += "\\r"; break;
		case '\t': result += "\\t"; break;
		default:
			if ('\x00' <= c && c <= '\x1f') {
				result += "\\u" + int_to_hex(static_cast<unsigned char>(c));
			} else {
				result += c;
			}
			break;
		}
	}
	return result;
}
