// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Shader.hpp"

#include "Scene/RendererObjectManager.hpp"
#include "Utils/StringExt.hpp"

#include <iomanip>

namespace Stone::Scene {

Shader::Shader(const std::string &content)
	: Object(), IRenderable(), _contentType(ContentType::SourceFile), _content(content) {
	if (string_ends_with(content, ".glsl")) {
		_contentType = ContentType::SourceFile;
	} else if (string_ends_with(content, ".spv") || string_ends_with(content, ".metal")) {
		_contentType = ContentType::CompiledFile;
	} else if (string_starts_with(content, "#version")) {
		_contentType = ContentType::SourceCode;
	} else if (string_contains_non_printable(content)) {
		_contentType = ContentType::CompiledCode;
	}
}

Shader::Shader(ContentType contentType, std::string content)
	: Object(), IRenderable(), _contentType(contentType), _content(std::move(content)) {
}

const char *Shader::getClassName() const {
	return "Shader";
}

std::ostream &Shader::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",function:\"" << _function << '"';
	switch (_contentType) {
	case ContentType::SourceCode: stream << ",source:\"" << _content << '"'; break;
	case ContentType::SourceFile: stream << ",source_file:\"" << _content << '"'; break;
	case ContentType::CompiledCode:
		stream << ",compiled:\"";
		for (char c : _content) {
			stream << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
		}
		stream << '"';
		break;
	case ContentType::CompiledFile: stream << ",compiled_file:\"" << _content << '"'; break;
	}
	if (closing_bracer) {
		stream << "}";
	}
	return stream;
}

void Shader::updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) {
	manager->updateShader(std::static_pointer_cast<Shader>(shared_from_this()));
}

std::pair<Shader::ContentType, const std::string &> Shader::getContent() const {
	return {_contentType, _content};
}

const std::string &Shader::getFunction() const {
	return _function;
}

void Shader::setFunction(const std::string &function) {
	_function = function;
	markDirty();
}

int Shader::getMaxLocation() const {
	return _maxLocation;
}

int Shader::getLocation(const std::string &name) const {
	auto it = _locations.find(name);
	if (it == _locations.end()) {
		return -1;
	}
	return it->second;
}

void Shader::setContent(ContentType contentType, std::string content) {
	_contentType = contentType;
	_content = std::move(content);
	markDirty();
}

void Shader::setLocation(const std::string &name, int location) {
	_locations[name] = location;
	if (location > _maxLocation) {
		_maxLocation = location;
	}
	markDirty();
}

} // namespace Stone::Scene
