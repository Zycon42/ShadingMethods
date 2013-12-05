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
#include "ShaderManager.h"
#include "Interfaces.h"

#include <memory>
#include <vector>
#include <unordered_map>

class Camera;
class IMaterial;
class Light;
class Scene;
class SceneNode;

namespace gl {

class ShadowMap;
class Renderer
{
public:
	Renderer();

	void initialize();

	ShaderManager* shaderManager();

	template <class T>
	std::unique_ptr<UniformBuffer<T>> createUniformBuffer() {
		return std::unique_ptr<UniformBuffer<T>>(new UniformBuffer<T>());
	}

	template <class T>
	std::unique_ptr<UniformBuffer<T>> createUniformBuffer(const T& initialData) {
		return std::unique_ptr<UniformBuffer<T>>(new UniformBuffer<T>(initialData));
	}

	/// Register scene node in render, so he will draw it.
	void registerRenderable(IRenderable* renderable);
	/// Stops drawing given node.
	void unregisterRenderable(IRenderable* renderable);

	void setViewport(const Viewport& viewport);

	/// Sets camera which will be used.
	void setCamera(Camera* camera);

	/// Sets light which will be used.
	void setLight(Light* light);

	/// Sets current scene to draw
	void setScene(Scene* scene);

	/// Draw single frame, drawing all registered nodes
	void drawFrame();

	void showBboxes() { m_showBboxes = true; }
	void hideBboxes() { m_showBboxes = false; }
	void toggleBboxVisibility() { m_showBboxes = !m_showBboxes; }
private:
	static const int CAMERA_BINDING_POINT = 0;
	static const int NODE_BINDING_POINT = 1;
	static const int MATERIAL_BINDING_POINT = 2;
	static const int LIGHT_BINDING_POINT = 3;

	static const size_t SHADOW_MAP_SIZE = 1024;
	static const int SHADOW_MAP_BINDING_POINT = 0;

	struct State
	{
		State() : shader(nullptr), materialUbo(nullptr), nodeUbo(nullptr) { }

		gl::ShaderProgram* shader;
		gl::IndexedBuffer* materialUbo;
		gl::IndexedBuffer* nodeUbo;
	};

	void drawBatch(RenderBatch& batch);
	void drawGeometry(GeometryBatch& geom);

	void drawSceneNodeBatches(SceneNode* node);
	void drawSceneNodeGeometry(SceneNode* node);

	void drawShadowMap();

	void drawBoundingBox(const BoundingBox& bbox);

	Viewport m_viewport;

	std::unordered_map<IRenderable*, RenderBatch> m_batches;
	State m_currentState;

	bool m_shadowMappingActive;
	std::unique_ptr<ShadowMap> m_shadowMap;

	Scene* m_scene;
	bool m_showBboxes;
	std::vector<BoundingBox> m_bboxDrawList;
};

class ShadowMap
{
public:
	ShadowMap(size_t size, std::shared_ptr<ShaderProgram> shader);
	~ShadowMap();

	GLuint fbo() {
		return m_fbo;
	}

	GLuint depthTexture() {
		return m_tex;
	}

	size_t size() const {
		return m_size;
	}

	gl::ShaderProgram* shader() {
		return m_shader.get();
	}
private:
	ShadowMap(const ShadowMap&);
	ShadowMap& operator=(const ShadowMap&);

	size_t m_size;
	std::shared_ptr<ShaderProgram> m_shader;

	GLuint m_fbo;
	GLuint m_tex;
};

}

#endif // !RENDERER_H
