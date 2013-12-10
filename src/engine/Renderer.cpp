/**
 * @file Renderer.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Renderer.h"

#include "Camera.h"
#include "BaseSceneObject.h"
#include "Light.h"
#include "Logging.h"
#include "Exception.h"
#include "Scene.h"
#include "BoundingBoxDrawer.h"

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

ShadowMap::~ShadowMap() {
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffers(1, &m_fbo);
}

Renderer::Renderer() : m_shadowMappingActive(false), m_showBboxes(false), 
	m_scene(nullptr), m_frameID(0) {

}

Renderer::~Renderer() { }

void Renderer::initialize() {
	m_bboxDrawer = std::unique_ptr<BoundingBoxDrawer>(
		new BoundingBoxDrawer(shaderManager()->getGlslProgram("simple"), &m_currentState)
	);

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

void Renderer::drawSceneNodeBatches(SceneNode* node) {
	// view frustum culling
	if (m_camera->viewFrustum().boundingBoxIntersetion(node->boundingBox()) == Frustum::Intersection::None)
		return;

	if (m_showBboxes)
		m_bboxDrawer->drawLinedSingle(node->boundingBox());

	if (node->isLeaf()) {
		for (size_t i = 0; i < node->numObjects(); ++i) {
			if (m_showBboxes)
				m_bboxDrawer->drawLinedSingle(node->object(i)->boundingBox());
			drawBatch(m_batches.at(node->object(i)));
		}
	} else {
		drawSceneNodeBatches(node->leftChild());
		drawSceneNodeBatches(node->rightChild());
	}
}

void Renderer::drawSceneNodeGeometry(SceneNode* node) {
	if (node->isLeaf()) {
		for (size_t i = 0; i < node->numObjects(); ++i) {
			drawGeometry(*m_batches.at(node->object(i)).geometry);
		}
	} else {
		drawSceneNodeGeometry(node->leftChild());
		drawSceneNodeGeometry(node->rightChild());
	}
}

void Renderer::drawFrame() {
	m_frameID++;

	// optional shadow map pass
	if (m_shadowMappingActive) {
		drawShadowMap();
	}

	// draw normal forward pass

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSceneWithOcclussionCulling(m_scene->rootNode());
	//drawSceneNodeBatches(m_scene->rootNode());

	VertexArrayObject::unbind();
}

void Renderer::setCamera(Camera* camera) {
	camera->uniformBuffer()->bind(CAMERA_BINDING_POINT, GL_UNIFORM_BUFFER);
	m_camera = camera;
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

void Renderer::setScene(Scene* scene) {
	m_scene = scene;
}

void Renderer::registerSceneObject(ISceneObject* renderable) {
	assert(renderable->material() != nullptr && renderable->mesh() != nullptr);

	RenderBatch batch;
	batch.shader = renderable->material()->shader();
	batch.materialUbo = renderable->material()->uniformBuffer();
	batch.nodeUbo = renderable->uniformBuffer();

	auto mesh = renderable->mesh();
	if (!mesh->isValid()) {
		LOG(ERROR) << "Renderable with invalid mesh registered to renderer.";
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

	m_batches.insert(std::make_pair(renderable, std::move(batch)));

	// group batches to minimize state changes
	/*std::sort(m_batches.begin(), m_batches.end(), [] (RenderBatch& b1, RenderBatch& b2) {
		if (b1.shader != b2.shader)
			return b1.shader < b2.shader;
		else if (b1.materialUbo != b2.materialUbo)
			return b1.materialUbo < b2.materialUbo;
		else
			return b1.nodeUbo < b2.nodeUbo;
	});*/
}

void Renderer::unregisterSceneObject(ISceneObject* renderable) {
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
	drawSceneNodeGeometry(m_scene->rootNode());
	VertexArrayObject::unbind();

	// unbound fbo and set viewport back
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(static_cast<int>(m_viewport.x), static_cast<int>(m_viewport.y), 
		static_cast<size_t>(m_viewport.width), static_cast<size_t>(m_viewport.height));
}

void Renderer::drawSceneWithOcclussionCulling(SceneNode* root) {
	traversalStack.push_back(root);
	while (!traversalStack.empty() || !queryQueue.empty()) {
		// process finished queries
		while (!queryQueue.empty() && (queryQueue.front()->query().isResultAvailable() || traversalStack.empty())) {
			// pop node from queue
			auto node = queryQueue.front();
			queryQueue.pop();

			// get query result (will wait to result if traversalStack is empty)
			int visible = GL_FALSE;
			node->query().getResult(&visible);
			if (visible) {
				pullUpVisibility(node);
				traverseNode(node);
			}
		}

		// hierarchical traversal
		if (!traversalStack.empty()) {
			// pop node from stack
			auto node = traversalStack.back();
			traversalStack.pop_back();
			
			// do frustum culling
			if (m_camera->viewFrustum().boundingBoxIntersetion(node->boundingBox()) != Frustum::Intersection::None) {
				// determine if node was previously visible
				bool wasVisible = node->isVisible() && (node->lastVisited() == m_frameID - 1);

				// update node visibility
				node->setVisibility(false);
				node->setLastVisited(m_frameID);

				// issue query for leafs and previously invisible
				if (node->isLeaf() || !wasVisible) {
					issueQuery(node);
				}

				// always traverse a node when it was visible
				if (wasVisible)
					traverseNode(node);
			}
		}
	}
}

void Renderer::pullUpVisibility(SceneNode* node) {
	while (node && !node->isVisible()) {
		node->setVisibility(true);
		node = node->parent();
	}
}

void Renderer::traverseNode(SceneNode* node) {
	if (m_showBboxes)
		m_bboxDrawer->drawLinedSingle(node->boundingBox());

	if (node->isLeaf()) {
		for (size_t i = 0; i < node->numObjects(); ++i) {
			drawBatch(m_batches.at(node->object(i)));
		}
	} else {
		auto left = node->leftChild();
		auto right = node->rightChild();

		// compute distance from camera position to children bboxes
		float distToLeft = left->boundingBox().distance(m_camera->position());
		float distToRight = right->boundingBox().distance(m_camera->position());

		// push children to stack ... farther child first
		if (distToLeft > distToRight) {
			traversalStack.push_back(left);
			traversalStack.push_back(right);
		} else {
			traversalStack.push_back(right);
			traversalStack.push_back(left);
		}
	}
}

void Renderer::issueQuery(SceneNode* node) {
	// disable writing to depth buffer and color buffer
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
	node->query().begin(GL_ANY_SAMPLES_PASSED);
	m_bboxDrawer->drawSingle(node->boundingBox());
	node->query().end(GL_ANY_SAMPLES_PASSED);

	// re enable writing to depth buffer and color buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// push to queue
	queryQueue.push(node);
}

}
