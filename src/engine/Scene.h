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

class Scene
{
public:
	explicit Scene(gl::Renderer* renderer) : m_renderer(renderer) {
	}

	gl::Renderer* renderer() {
		return m_renderer;
	}

	void changeRenderer(gl::Renderer* renderer);

	void addNode(std::unique_ptr<Node> node);
	void removeNode(size_t index);
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
