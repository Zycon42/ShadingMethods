/**
 * @file Node.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef NODE_H
#define NODE_H

#include "Scene.h"
#include "Mesh.h"
#include "Material.h"

#include <glm/glm.hpp>

/**
 * Scene node.
 * Contains mesh, material and node transforms
 */
class Node
{
public:
	/// Create node from mesh and material.
	Node(std::shared_ptr<Mesh> mesh, std::shared_ptr<IMaterial> material);

	Mesh* mesh() {
		return m_mesh.get();
	}
	void setMesh(std::shared_ptr<Mesh> mesh);

	IMaterial* material() {
		return m_material.get();
	}
	void setMaterial(std::shared_ptr<IMaterial> material);

	const glm::mat4& modelMatrix() const {
		return m_buffer->data().model;
	}
	void setModelMatrix(const glm::mat4& m);

	gl::IndexedBuffer* uniformBuffer() {
		return m_buffer->internalBuffer();
	}

	void addedToScene(Scene* scene);
	void sceneRendererChanged();
	void removedFromScene();
private:
	void createUniformBuffer(gl::Renderer* renderer);

	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<IMaterial> m_material;

	struct BufferData
	{
		glm::mat4 model;
		glm::mat4 normalMatrix;
	};

	std::unique_ptr<UniformBuffer<BufferData>> m_buffer;

	Scene* m_scene;
};

#endif // !NODE_H
