// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/RenderElement/IRenderElement.hpp"

#include <glm/vec3.hpp>
#include <unordered_map>

namespace Stone::Scene {

class Texture;

/**
 * @brief The Material class represents a material used for rendering objects in the scene.
 *
 * It contains various parameters such as textures, vectors, and scalars that can be used
 * to configure the appearance of rendered objects.
 */
class Material : public Core::Object, public IRenderElement {
public:
	Material() = default;
	Material(const Material &other) = default;

	~Material() override = default;

	/**
	 * @brief Get the class name of the Material object.
	 *
	 * @return The class name as a null-terminated string.
	 */
	[[nodiscard]] const char *getClassName() const override;

	/**
	 * @brief Write the Material object to an output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing bracer after writing the Material object.
	 * @return The output stream after writing the Material object.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Update the render object associated with the Material.
	 *
	 * @param manager The RendererObjectManager used to update the render object.
	 */
	void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) override;

	/**
	 * @brief Set a texture parameter for the Material.
	 *
	 * @param name The name of the texture parameter.
	 * @param texture The texture to set.
	 */
	void setTextureParameter(const std::string &name, std::shared_ptr<Texture> texture);

	/**
	 * @brief Get a texture parameter from the Material.
	 *
	 * @param name The name of the texture parameter.
	 * @return The texture parameter as a shared pointer to Texture.
	 */
	[[nodiscard]] std::shared_ptr<Texture> getTextureParameter(const std::string &name) const;

	/**
	 * @brief Set a vector parameter for the Material.
	 *
	 * @param name The name of the vector parameter.
	 * @param vector The vector to set.
	 */
	void setVectorParameter(const std::string &name, const glm::vec3 &vector);

	/**
	 * @brief Get a vector parameter from the Material.
	 *
	 * @param name The name of the vector parameter.
	 * @return The vector parameter as a glm::vec3.
	 */
	[[nodiscard]] glm::vec3 getVectorParameter(const std::string &name) const;

	/**
	 * @brief Set a scalar parameter for the Material.
	 *
	 * @param name The name of the scalar parameter.
	 * @param scalar The scalar value to set.
	 */
	void setScalarParameter(const std::string &name, float scalar);

	/**
	 * @brief Get a scalar parameter from the Material.
	 *
	 * @param name The name of the scalar parameter.
	 * @return The scalar parameter as a float.
	 */
	[[nodiscard]] float getScalarParameter(const std::string &name) const;

protected:
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures; /**< Map of texture parameters. */
	std::unordered_map<std::string, glm::vec3> _vectors;				 /**< Map of vector parameters. */
	std::unordered_map<std::string, float> _scalars;					 /**< Map of scalar parameters. */
};

} // namespace Stone::Scene
