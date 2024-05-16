// Copyright 2024 Stone-Engine

#include "Scene/RenderElement/Shader.hpp"

#include "Scene/RendererObjectManager.hpp"

#include <iomanip>

namespace Stone::Scene {

Shader::Shader(ContentType contentType, std::string content)
	: Object(), IRenderElement(), _contentType(contentType), _content(std::move(content)) {
}

const char *Shader::getClassName() const {
	return "Shader";
}

std::ostream &Shader::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	switch (_contentType) {
	case ContentType::SourceCode: stream << "source:\"" << _content << '"'; break;
	case ContentType::SourceFile: stream << "sourceFile:\"" << _content << '"'; break;
	case ContentType::CompiledCode:
		stream << "compiled:\"";
		for (char c : _content) {
			stream << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
		}
		stream << '"';
		break;
	case ContentType::CompiledFile: stream << "compiledFile:\"" << _content << '"'; break;
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
