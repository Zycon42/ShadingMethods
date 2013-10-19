/**
 * @file Renderer.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Renderer.h"

#include "Camera.h"
#include "Node.h"
#include "Light.h"
#include "Logging.h"
#include "Exception.h"

#include <GL/glew.h>

#include <algorithm>
#include <cassert>

namespace gl {

ShadowMap::ShadowMap(size_t size, std::shared_ptr<ShaderProgram> shader) 
	: m_size(size), m_shader(std::move(shader)) 
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// create depth buffer texture
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	// use texture as depth buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_tex, 0);

	// do not draw color buffer
	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw Exception("glCheckFramebufferStatus returns error");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::Renderer() : m_shadowMappingActive(false) {

}

void Renderer::initialize() {
	glEnable(GL_DEPTH_TEST);
}

ShaderManager* Renderer::shaderManager() {
	static ShaderManager manager;
	return &manager;
}

void Renderer::setViewport(const Viewport& viewport) {
	glViewport(static_cast<int>(viewport.x), static_cast<int>(viewport.y), 
		static_cast<size_t>(viewport.width), static_cast<size_t>(viewport.height));
	glDepthRange(viewport.znear, viewport.zfar);

	m_viewport = viewport;
}

void Renderer::drawFrame() {
	if (m_shadowMappingActive) {
		drawShadowMap();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& batch : m_batches) {
		drawBatch(batch);
	}
	VertexArrayObject::unbind();
}

void Renderer::setCamera(Camera* camera) {
	camera->uniformBuffer()->bind(CAMERA_BINDING_POINT, GL_UNIFORM_BUFFER);
}

void Renderer::setLight(Light* light) {
	light->uniformBuffer()->bind(LIGHT_BINDING_POINT,  GL_UNIFORM_BUFFER);

	// create shadow map if we don't have one and new light is shadow source.
	if (!m_shadowMappingActive && light->isShadowSource()) {
		m_shadowMap = std::unique_ptr<ShadowMap>(
			new ShadowMap(SHADOW_MAP_SIZE, shaderManager()->getGlslProgram("shadowmap"))
		);

		m_shadowMappingActive = true;
	// release shadow map if light is no longer shadow source.
	} else if (m_shadowMappingActive && !light->isShadowSource()) {
		m_shadowMap = nullptr;
		m_shadowMappingActive = false;
	}
}

void Renderer::registerSceneNode(Node* node) {
	assert(node->material() != nullptr && node->mesh() != nullptr);

	RenderBatch batch;
	batch.shader = node->material()->shader();
	batch.materialUbo = node->material()->uniformBuffer();
	batch.nodeUbo = node->uniformBuffer();

	auto mesh = node->mesh();
	if (!mesh->isValid()) {
		LOG(ERROR) << "Scene node with invalid mesh registered to renderer.";
		return;
	}

	// create OpenGL geometry i.e. vao, vbo etc.

	auto vbo = std::make_shared<Buffer>();
	vbo->loadData(mesh->vertexData().data(), mesh->vertexData().size());

	auto geom = new GeometryBatch();
	geom->vao().bind();
	geom->setPrimitiveType(mesh->primitiveType());
	geom->setVertices(vbo, mesh->vertexCount(), mesh->vertexLayout());
	if (mesh->isIndexed()) {
		auto ebo = std::make_shared<Buffer>();
		ebo->loadData(mesh->indices().data(), mesh->indices().size() * sizeof(uint32_t));
		geom->setIndices(ebo, GL_UNSIGNED_INT, mesh->indices().size());
	}
	VertexArrayObject::unbind();
	batch.geometry.reset(geom);

	m_batches.push_back(std::move(batch));

	// group batches to minimize state changes
	std::sort(m_batches.begin(), m_batches.end(), [] (RenderBatch& b1, RenderBatch& b2) {
		if (b1.shader != b2.shader)
			return b1.shader < b2.shader;
		else if (b1.materialUbo != b2.materialUbo)
			return b1.materialUbo < b2.materialUbo;
		else
			return b1.nodeUbo < b2.nodeUbo;
	});
}

void Renderer::unregisterSceneNode(Node* node) {
	LOG(ERROR) << "Renderer::unregisterSceneNode not implemented.";
}

void Renderer::drawBatch(RenderBatch& batch) {
	if (batch.shader != m_currentState.shader) {
		batch.shader->use();
		m_currentState.shader = batch.shader;
	}

	if (batch.materialUbo != m_currentState.materialUbo) {
		batch.materialUbo->bind(MATERIAL_BINDING_POINT, GL_UNIFORM_BUFFER);
		m_currentState.materialUbo = batch.materialUbo;
	}

	if (batch.nodeUbo != m_currentState.nodeUbo) {
		batch.nodeUbo->bind(NODE_BINDING_POINT, GL_UNIFORM_BUFFER);
		m_currentState.nodeUbo = batch.nodeUbo;
	}

	// bind shadow map to texture unit
	if (m_shadowMappingActive) {
		glActiveTexture(GL_TEXTURE0 + SHADOW_MAP_BINDING_POINT);
		glBindTexture(GL_TEXTURE_2D, m_shadowMap->depthTexture());
	}

	drawGeometry(*batch.geometry);
}

void Renderer::drawGeometry(GeometryBatch& geom) {
	geom.vao().bind();
	if (geom.hasElements())
		glDrawElements(geom.drawMode(), geom.indexCount(), geom.elementsType(), nullptr);
	else
		glDrawArrays(geom.drawMode(), 0, geom.vertexCount());
}

void Renderer::drawShadowMap() {
	// bind fbo and set its viewport
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->fbo());
	glViewport(0, 0, m_shadowMap->size(), m_shadowMap->size());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shadowMap->shader()->use();
	m_currentState.shader = m_shadowMap->shader();

	// draw only geometry
	for (auto& batch : m_batches) {
		drawGeometry(*batch.geometry);
	}
	VertexArrayObject::unbind();

	// unbound fbo and set viewport back
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(static_cast<int>(m_viewport.x), static_cast<int>(m_viewport.y), 
		static_cast<size_t>(m_viewport.width), static_cast<size_t>(m_viewport.height));
}

}
