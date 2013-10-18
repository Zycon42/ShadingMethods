/**
 * @file Renderer.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "Common.h"
#include "UniformBuffer.h"
#include "RenderBatch.h"

#include <memory>
#include <vector>

class Camera;
class IMaterial;
class Node;
class Light;

namespace gl {

class Renderer
{
public:
	Renderer();

	template <class T>
	std::unique_ptr<UniformBuffer<T>> createUniformBuffer() {
		return std::unique_ptr<UniformBuffer<T>>(new UniformBuffer<T>());
	}

	template <class T>
	std::unique_ptr<UniformBuffer<T>> createUniformBuffer(T&& initialData) {
		return std::unique_ptr<UniformBuffer<T>>(new UniformBuffer<T>(std::forward<T>(initialData)));
	}

	/// Register scene node in render, so he will draw it.
	void registerSceneNode(Node* node);
	/// Stops drawing given node.
	void unregisterSceneNode(Node* node);

	void setViewport(const Viewport& viewport);

	/// Sets camera which will be used.
	void setCamera(Camera* camera);

	/// Sets light which will be used.
	void setLight(Light* light);

	/// Draw single frame, drawing all registered nodes
	void drawFrame();
private:
	static const int CAMERA_BINDING_POINT = 0;
	static const int NODE_BINDING_POINT = 1;
	static const int MATERIAL_BINDING_POINT = 2;
	static const int LIGHT_BINDING_POINT = 3;

	struct State
	{
		State() : shader(nullptr), materialUbo(nullptr), nodeUbo(nullptr) { }

		gl::ShaderProgram* shader;
		gl::IndexedBuffer* materialUbo;
		gl::IndexedBuffer* nodeUbo;
	};

	void drawBatch(RenderBatch& batch);

	std::vector<RenderBatch> batches;
	State currentState;
};

}

#endif // !RENDERER_H
