/**
 * @file Scene.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef SCENE_H
#define SCENE_H

#include "Renderer.h"
#include "BVH.h"

#include <memory>
#include <vector>

class BaseSceneObject;
class SceneNode;

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

	void setStaticGeometry(std::vector<std::shared_ptr<BaseSceneObject>> nodes);

	SceneNode* rootNode() {
		return m_root.get();
	}
private:
	Scene(const Scene&);
	Scene& operator=(Scene);

	SceneNode* buildTree(size_t iBvhNode, SceneNode* parent = nullptr);

	friend class SceneNode;

	gl::Renderer* m_renderer;
	std::vector<std::shared_ptr<BaseSceneObject>> m_objects;
	std::unique_ptr<BVH> m_bvh;
	std::unique_ptr<SceneNode> m_root;
};

class SceneNode
{
public:
	bool isLeaf() const {
		return m_bvhNode->rightOffset == 0;
	}

	size_t numObjects() const {
		return m_bvhNode->numObjects;
	}

	BaseSceneObject* object(size_t i) {
		return m_scene->m_objects[m_bvhNode->start + i].get();
	}

	BoundingBox boundingBox() const {
		return m_bvhNode->bbox;
	}

	SceneNode* leftChild() const {
		return m_left.get();
	}

	SceneNode* rightChild() const {
		return m_right.get();
	}

	SceneNode* parent() const {
		return m_parent;
	}

	bool isVisible() const {
		return m_visible;
	}

	void setVisibility(bool visible) {
		m_visible = visible;
	}

	uint32_t lastVisited() const {
		return m_lastVisited;
	}

	void setLastVisited(uint32_t val) {
		m_lastVisited = val;
	}
private:
	SceneNode(Scene* scene, BVH::Node* node, SceneNode* parent)
		: m_scene(scene), m_bvhNode(node), m_parent(parent), m_visible(true), m_lastVisited(0) { }

	friend class Scene;

	Scene* m_scene;
	BVH::Node* m_bvhNode;
	SceneNode* m_parent;
	std::unique_ptr<SceneNode> m_left, m_right;

	bool m_visible;
	uint32_t m_lastVisited;
};

#endif // !SCENE_H
