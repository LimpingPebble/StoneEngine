// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/RenderContext.hpp"

namespace Stone::Scene {

/**
 * @brief Interface for render behaviour
 *
 * This interface is used to define the render behaviour of a renderable element.
 * The render behaviour is the way the element is rendered in the scene.
 *
 * ```
 * class VulkanMesh : IRendererObject
 * {
 * public:
 *    VulkanMesh(std::shared_ptr<Mesh> mesh);
 *    virtual void render(RenderContext &context) override;
 * }
 * ```
 */
class IRendererObject {
public:
	virtual ~IRendererObject() = default;

	virtual void render(RenderContext &context) = 0;
};

class ISceneRenderer;
class RendererObjectManager;

class IRenderElement {
public:
	IRenderElement() : _rendererObject(nullptr), _dirty(true) {
	}

	IRenderElement(const IRenderElement &other) : _rendererObject(other._rendererObject), _dirty(true) {
	}

	virtual ~IRenderElement() = default;

	virtual void updateRenderObject(const std::shared_ptr<RendererObjectManager> &manager) = 0;

	[[nodiscard]] bool isDirty() const {
		return _dirty;
	}

	void markDirty() {
		_dirty = true;
	}

	template <typename T>
	[[nodiscard]] std::shared_ptr<T> getRendererObject() const {
		return std::dynamic_pointer_cast<T>(_rendererObject);
	}

protected:
	friend class RendererObjectManager;

	void setRendererObject(const std::shared_ptr<IRendererObject> &rendererObject) {
		_rendererObject = rendererObject;
	}

	void markUndirty() {
		_dirty = false;
	}

	std::shared_ptr<IRendererObject> _rendererObject;
	bool _dirty;
};

} // namespace Stone::Scene
