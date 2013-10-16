/**
 * @file Renderer.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Renderer.h"

#include "Camera.h"
#include "Node.h"
#include "Logging.h"

#include <GL/glew.h>

#include <algorithm>
#include <cassert>

namespace gl {

Renderer::Renderer() {

}

void Renderer::setViewport(const Viewport& viewport) {
	glViewport(static_cast<int>(viewport.x), static_cast<int>(viewport.y), 
		static_cast<size_t>(viewport.width), static_cast<size_t>(viewport.height));
	glDepthRange(viewport.znear, viewport.zfar);
}

void Renderer::drawFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& batch : batches) {
		drawBatch(batch);
	}
	VertexArrayObject::unbind();
}

void Renderer::setCamera(Camera* camera) {
	camera->uniformBuffer()->bind(CAMERA_BINDING_POINT, GL_UNIFORM_BUFFER);
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

	batches.push_back(std::move(batch));

	// group batches to minimize state changes
	std::sort(batches.begin(), batches.end(), [] (RenderBatch& b1, RenderBatch& b2) {
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
	if (batch.shader != currentState.shader) {
		batch.shader->use();
		currentState.shader = batch.shader;
	}

	if (batch.materialUbo != currentState.materialUbo) {
		batch.materialUbo->bind(MATERIAL_BINDING_POINT, GL_UNIFORM_BUFFER);
		currentState.materialUbo = batch.materialUbo;
	}

	if (batch.nodeUbo != currentState.nodeUbo) {
		batch.nodeUbo->bind(NODE_BINDING_POINT, GL_UNIFORM_BUFFER);
		currentState.nodeUbo = batch.nodeUbo;
	}

	auto geom = batch.geometry.get();
	geom->vao().bind();
	if (geom->hasElements())
		glDrawElements(geom->drawMode(), geom->indexCount(), geom->elementsType(), nullptr);
	else
		glDrawArrays(geom->drawMode(), 0, geom->vertexCount());
}

}
