/**
 * @file Scene.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef SCENE_H
#define SCENE_H

#include "Renderer.h"

#include <memory>
#include <vector>

class Node;

/**
 * Scene contains nodes which are renderable objects.
 */
class Scene
{
public:
	explicit Scene(gl::Renderer* renderer) : m_renderer(renderer) {
	}

	gl::Renderer* renderer() {
		return m_renderer;
	}

	/// Change renderer which scene is bound to.
	void changeRenderer(gl::Renderer* renderer);

	/// Add node to scene.
	void addNode(std::unique_ptr<Node> node);
	/// Remove node from scene.
	void removeNode(size_t index);
	/// Extracts node from scene i.e. node is moved out.
	std::unique_ptr<Node> extractNode(size_t index);

	void nodeGeometryChanged(Node* node);
	void nodeMaterialChanged(Node* node);
private:
	Scene(const Scene&);
	Scene& operator=(Scene);

	gl::Renderer* m_renderer;
	std::vector<std::unique_ptr<Node>> m_nodes;
};

#endif // !SCENE_H
