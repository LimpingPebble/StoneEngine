// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"

#include <glm/vec3.hpp>
#include <map>

namespace Stone::Scene {

class Texture;

class Material : public Core::Object, public IRenderElement {
public:
	Material() = default;
	Material(const Material &other) = default;

	~Material() override = default;

	[[nodiscard]] const char *getClassName() const override;
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) override;

	void setTextureParameter(const std::string &name, std::shared_ptr<Texture> texture);
	[[nodiscard]] std::shared_ptr<Texture> getTextureParameter(const std::string &name) const;

	void setVectorParameter(const std::string &name, const glm::vec3 &vector);
	[[nodiscard]] glm::vec3 getVectorParameter(const std::string &name) const;

	void setScalarParameter(const std::string &name, float scalar);
	[[nodiscard]] float getScalarParameter(const std::string &name) const;

protected:
	std::map<std::string, std::shared_ptr<Texture>> _textures;
	std::map<std::string, glm::vec3> _vectors;
	std::map<std::string, float> _scalars;
};

} // namespace Stone::Scene
