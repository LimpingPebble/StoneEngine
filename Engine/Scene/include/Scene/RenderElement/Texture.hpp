// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"

namespace Stone::Core {
class Image;
}

namespace Stone::Scene {

class Texture : public Core::Object, public IRenderElement {
public:
	Texture();
	Texture(const Texture &other);

	virtual ~Texture();

	[[nodiscard]] const char *getClassName() const override;
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void generateRenderBehaviour(std::shared_ptr<ISceneRenderer> renderer) override;

	void setImage(std::shared_ptr<Core::Image> image);
	[[nodiscard]] std::shared_ptr<Core::Image> getImage() const;

protected:
	std::shared_ptr<Core::Image> _image;
};

} // namespace Stone::Scene
