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
	Texture() = default;
	Texture(const Texture &other) = default;

	~Texture() override = default;

	[[nodiscard]] const char *getClassName() const override;
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	void setImage(std::shared_ptr<Core::Image> image);
	[[nodiscard]] std::shared_ptr<Core::Image> getImage() const;

protected:
	std::shared_ptr<Core::Image> _image;
};

} // namespace Stone::Scene
