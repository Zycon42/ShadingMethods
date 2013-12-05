/**
 * @file Node.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "BaseSceneObject.h"

BaseSceneObject::BaseSceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<IMaterial> material)
	: m_mesh(std::move(mesh)), m_material(std::move(material)), m_buffer(nullptr), m_scene(nullptr) { }

void BaseSceneObject::setModelMatrix(const glm::mat4& m) {
	BufferData data = { m,  glm::transpose(glm::inverse(m)) };
	if (m_buffer) {
		m_buffer->setData(data);
		m_buffer->flushData();
	} else {
		m_memoryData = data;
	}
}

void BaseSceneObject::addedToScene(Scene* scene) {
	m_scene = scene;
	createUniformBuffer(scene->renderer());
}

void BaseSceneObject::sceneRendererChanged() {
	createUniformBuffer(m_scene->renderer());
}

void BaseSceneObject::removedFromScene() {
	m_buffer = nullptr;
	m_scene = nullptr;
}

void BaseSceneObject::createUniformBuffer(gl::Renderer* renderer) {
	m_buffer = renderer->createUniformBuffer<BufferData>(m_memoryData);
}

void BaseSceneObject::setMesh(std::shared_ptr<Mesh> mesh) {
	m_mesh = std::move(mesh);
}

void BaseSceneObject::setMaterial(std::shared_ptr<IMaterial> material) {
	m_material = std::move(material);
}
