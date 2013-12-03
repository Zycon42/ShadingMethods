/**
 * @file Scene.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Scene.h"
#include "Node.h"
#include "Logging.h"

#include <SDL.h>

double getTime() {
	static uint64_t freq;
	static bool first = true;
	if (first) {
		first = false;
		freq = SDL_GetPerformanceFrequency();
	}

	uint64_t counter = SDL_GetPerformanceCounter();
	return static_cast<double>(counter) / static_cast<double>(freq);
}

void Scene::changeRenderer(gl::Renderer* renderer) {
	m_renderer = renderer;
	for (auto& node : m_objects) {
		node->sceneRendererChanged();
	}
}

void Scene::setStaticGeometry(std::vector<std::shared_ptr<AbstractNode>> objects) {
	m_objects = std::move(objects);
	for (auto& obj : m_objects) {
		obj->addedToScene(this);
		m_renderer->registerRenderable(obj.get());
	}

	volatile double t1 = getTime();
	m_bvh = std::unique_ptr<BVH>(BVH::build(m_objects.begin(), m_objects.end()));
	m_root = std::unique_ptr<SceneNode>(buildTree(0));
	volatile double t2 = getTime();

	LOG(INFO) << "Building BVH took: " << (t2 - t1) * 1000 << " ms";
}

SceneNode* Scene::buildTree(size_t iBvhNode, SceneNode* parent) {
	BVH::Node* bvhNode = &m_bvh->nodes()[iBvhNode];
	SceneNode* node = new SceneNode(this, bvhNode, parent);
	if (bvhNode->rightOffset != 0) {
		node->m_left = std::unique_ptr<SceneNode>(buildTree(iBvhNode + 1, node));
		node->m_right = std::unique_ptr<SceneNode>(buildTree(iBvhNode + bvhNode->rightOffset, node));
	}
	return node;
}
