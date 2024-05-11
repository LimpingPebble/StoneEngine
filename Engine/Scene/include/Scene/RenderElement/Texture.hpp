// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"

namespace Stone::Core {
class Image;
}

namespace Stone::Scene {

/**
 * @brief The Texture class represents a texture used in rendering.
 */
class Texture : public Core::Object, public IRenderElement {
public:
	Texture() = default;
	Texture(const Texture &other) = default;

	~Texture() override = default;

	/**
	 * @brief Get the class name of the Texture object.
	 *
	 * @return The class name as a null-terminated string.
	 */
	[[nodiscard]] const char *getClassName() const override;

	/**
	 * @brief Write the Texture object to an output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing brace after the object.
	 * @return The output stream after writing the Texture object.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Update the render object associated with the Texture.
	 *
	 * @param manager The RendererObjectManager used to update the render object.
	 */
	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	/**
	 * @brief Set the image associated with the Texture.
	 *
	 * @param image The image to set.
	 */
	void setImage(std::shared_ptr<Core::Image> image);

	/**
	 * @brief Get the image associated with the Texture.
	 *
	 * @return The image associated with the Texture.
	 */
	[[nodiscard]] std::shared_ptr<Core::Image> getImage() const;

protected:
	std::shared_ptr<Core::Image> _image; /**< The image associated with the Texture. */
};

} // namespace Stone::Scene
