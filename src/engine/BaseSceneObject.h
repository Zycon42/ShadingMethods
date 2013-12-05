/**
 * @file Node.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef BASE_SCENE_OBJECT_H
#define BASE_SCENE_OBJECT_H

#include "Interfaces.h"
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"

#include <glm/glm.hpp>

/**
 * Scene object.
 * Contains mesh, material and node transforms
 */
class BaseSceneObject : public ISceneObject
{
public:
	/// Create node from mesh and material.
	BaseSceneObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<IMaterial> material);

	virtual Mesh* mesh() {
		return m_mesh.get();
	}
	void setMesh(std::shared_ptr<Mesh> mesh);

	virtual IMaterial* material() {
		return m_material.get();
	}
	void setMaterial(std::shared_ptr<IMaterial> material);

	const glm::mat4& modelMatrix() const {
		if (m_buffer)
			return m_buffer->data().model;
		else
			return m_memoryData.model;
	}
	void setModelMatrix(const glm::mat4& m);

	virtual gl::IndexedBuffer* uniformBuffer() {
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
	BufferData m_memoryData;
	Scene* m_scene;
};

#endif // !BASE_SCENE_OBJECT_H
