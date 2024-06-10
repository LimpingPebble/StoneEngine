// Copyright 2024 Stone-Engine

#include "Utils/StringExt.hpp"

#include <algorithm>
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
	return std::any_of(str.begin(), str.end(), [](char c) {
		return !std::isprint(static_cast<unsigned char>(c)) && !std::isspace(static_cast<unsigned char>(c));
	});
}
