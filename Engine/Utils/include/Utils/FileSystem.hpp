// Copyright 2024 Stone-Engine

#pragma once

#include <string>
#include <vector>

namespace Stone::Utils {

std::vector<char> readBinaryFile(const std::string &filename);
std::string readTextFile(const std::string &filename);
void writeFile(const std::string &filename, const std::vector<char> &data);

} // namespace Stone::Utils
