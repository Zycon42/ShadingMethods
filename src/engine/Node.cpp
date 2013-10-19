/**
 * @file Node.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Node.h"

Node::Node(std::shared_ptr<Mesh> mesh, std::shared_ptr<IMaterial> material)
	: m_mesh(std::move(mesh)), m_material(std::move(material)), m_buffer(nullptr), m_scene(nullptr) { }

void Node::setModelMatrix(const glm::mat4& m) {
	BufferData data = { m,  glm::transpose(glm::inverse(m)) };
	m_buffer->setData(data);
	m_buffer->flushData();
}

void Node::addedToScene(Scene* scene) {
	m_scene = scene;
	createUniformBuffer(scene->renderer());
}

void Node::sceneRendererChanged() {
	createUniformBuffer(m_scene->renderer());
}

void Node::removedFromScene() {
	m_buffer = nullptr;
	m_scene = nullptr;
}

void Node::createUniformBuffer(gl::Renderer* renderer) {
	m_buffer = renderer->createUniformBuffer<BufferData>();
}

void Node::setMesh(std::shared_ptr<Mesh> mesh) {
	m_mesh = std::move(mesh);
	m_scene->nodeGeometryChanged(this);
}

void Node::setMaterial(std::shared_ptr<IMaterial> material) {
	m_material = std::move(material);
	m_scene->nodeMaterialChanged(this);
}
