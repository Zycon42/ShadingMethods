/**
 * @file Node.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Node.h"

AbstractNode::AbstractNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<IMaterial> material)
	: m_mesh(std::move(mesh)), m_material(std::move(material)), m_buffer(nullptr), m_scene(nullptr) { }

void AbstractNode::setModelMatrix(const glm::mat4& m) {
	BufferData data = { m,  glm::transpose(glm::inverse(m)) };
	if (m_buffer) {
		m_buffer->setData(data);
		m_buffer->flushData();
	} else {
		m_memoryData = data;
	}
}

void AbstractNode::addedToScene(Scene* scene) {
	m_scene = scene;
	createUniformBuffer(scene->renderer());
}

void AbstractNode::sceneRendererChanged() {
	createUniformBuffer(m_scene->renderer());
}

void AbstractNode::removedFromScene() {
	m_buffer = nullptr;
	m_scene = nullptr;
}

void AbstractNode::createUniformBuffer(gl::Renderer* renderer) {
	m_buffer = renderer->createUniformBuffer<BufferData>(m_memoryData);
}

void AbstractNode::setMesh(std::shared_ptr<Mesh> mesh) {
	m_mesh = std::move(mesh);
}

void AbstractNode::setMaterial(std::shared_ptr<IMaterial> material) {
	m_material = std::move(material);
}
