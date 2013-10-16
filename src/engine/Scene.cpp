/**
 * @file Scene.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Scene.h"
#include "Node.h"

void Scene::changeRenderer(gl::Renderer* renderer) {
	m_renderer = renderer;
	for (auto& node : m_nodes) {
		node->sceneRendererChanged();
	}
}

void Scene::addNode(std::unique_ptr<Node> node) {
	node->addedToScene(this);
	m_renderer->registerSceneNode(node.get());

	m_nodes.emplace_back(std::move(node));
}

void Scene::removeNode(size_t index) {
	auto nodeIt = m_nodes.begin() + index;
	m_renderer->unregisterSceneNode(nodeIt->get());
	m_nodes.erase(nodeIt);
}

std::unique_ptr<Node> Scene::extractNode(size_t index) {
	auto nodeIt = m_nodes.begin() + index;
	auto node = std::move(*nodeIt);
	m_nodes.erase(nodeIt);

	node->removedFromScene();
	return node;
}

void Scene::nodeGeometryChanged(Node* node) {
	// TODO:
}

void Scene::nodeMaterialChanged(Node* node) {
	// TODO:
}
