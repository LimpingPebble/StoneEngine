// Copyright 2024 Stone-Engine

#include "Utils/FileSystem.hpp"

#include <filesystem>
#include <fstream>

namespace Stone::Utils {

std::vector<char> readBinaryFile(const std::string &filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

	file.close();

	return buffer;
}

std::string readTextFile(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return content;
}

void writeFile(const std::string &filename, const std::vector<char> &data) {
	std::ofstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	file.write(data.data(), static_cast<std::streamsize>(data.size()));

	file.close();
}

} // namespace Stone::Utils
