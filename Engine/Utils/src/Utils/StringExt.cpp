// Copyright 2024 Stone-Engine

#include "Utils/StringExt.hpp"

#include <cctype>


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
	for (char c : str) {
		if (!isprint(static_cast<unsigned char>(c)) && !isspace(static_cast<unsigned char>(c))) {
			return true;
		}
	}
	return false;
}
