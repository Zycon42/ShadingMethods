/**
 * @file BVH.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef BVH_H
#define BVH_H

#include "Interfaces.h"

#include <vector>
#include <memory>
#include <stdexcept>

/// Static Bounding volume hierarchy
class BVH
{
public:
	struct Node
	{
		size_t start;
		size_t numObjects;
		size_t rightOffset;
		BoundingBox bbox;
	};

	class Iterator
	{
	public:
		Iterator(BVH* bvh, size_t i) : m_bvh(bvh), m_index(i) {
			stack.reserve(64);
			stack.push_back(i);
			++*this;
		}

		Iterator leftChild() {
			return Iterator(m_bvh, m_index + 1);
		}

		Iterator rightChild() {
			return Iterator(m_bvh, m_index + m_bvh->m_nodes[m_index].rightOffset);
		}

		Node& operator*() {
			return m_bvh->m_nodes[m_index];
		}

		Node* operator->() {
			return &m_bvh->m_nodes[m_index];
		}

		Iterator& operator++() {
			if (!stack.empty()) {
				m_index = stack.back();
				stack.pop_back();

				auto& node = m_bvh->m_nodes[m_index];
				// non-leaf
				if (node.rightOffset != 0) {
					stack.push_back(m_index + node.rightOffset);
					stack.push_back(m_index + 1);
				}
			}

			return *this;
		}

		Iterator operator++(int) {
			Iterator it(*this);
			++*this;
			return it;
		}

		bool operator==(const Iterator& other) {
			return m_bvh == other.m_bvh && m_index == other.m_index;
		}

		bool operator!=(const Iterator& other) {
			return !(*this == other);
		}
	private:
		BVH* m_bvh;
		size_t m_index;
		std::vector<size_t> stack;
	};

	template <class RandomAccessIterator>
	static BVH* build(RandomAccessIterator first, RandomAccessIterator last, size_t leafSize = 4);

	Iterator begin() {
		return Iterator(this, 0);
	}

	Iterator end() {
		return Iterator(this, m_nodes.size() - 1);
	}

	std::vector<Node>& nodes() {
		return m_nodes;
	}
private:
	static const size_t UNTOUCHED = ~0;
	static const size_t TOUCHED_TWICE = UNTOUCHED - 2;
	static const size_t ROOT_PARENT = ~0;

	struct BuildEntry
	{
		// index of parent
		size_t parent;
		// The range of objects in the object list covered by this node.
		size_t start, end;
	};

	size_t m_leafSize;
	size_t m_numLeafs;		/// number of leafs in BVH
	std::vector<Node> m_nodes;
};

template <class RandomAccessIterator>
BVH* BVH::build(RandomAccessIterator first, RandomAccessIterator last, size_t leafSize) {
	if (last - first <= 0)
		throw std::runtime_error("BVH::build cannot be caled on empty range");

	size_t numLeafs = 0;
	std::vector<Node> nodes;

	// create stack and reserve some space on it
	std::vector<BuildEntry> stack;
	stack.reserve(128);

	// push root to stack
	BuildEntry be = { ROOT_PARENT, 0, last - first };
	stack.push_back(be);

	nodes.reserve((last - first) * 2);

	while (!stack.empty()) {
		BuildEntry stackNode = stack.back();
		stack.pop_back();
		size_t start = stackNode.start;
		size_t end = stackNode.end;
		size_t numObjects = end - start;

		// calculate bounding box for this node
		BoundingBox bb(first[start]->boundingBox());
		BoundingBox bc(first[start]->centroid());
		for (size_t i = start + 1; i < end; ++i) {
			bb.expandToInclude(first[i]->boundingBox());
			bc.expandToInclude(first[i]->centroid());
		}

		Node node = { stackNode.start, numObjects, UNTOUCHED, bb };

		// if number of primitives at this point is less than desired
		// leaf size then this node will become leaf (Signified by rightOffset == 0)
		if (numObjects <= leafSize) {
			node.rightOffset = 0;
			numLeafs++;
		}

		nodes.push_back(node);

		// Child touches parent ... do not do this for root
		if (stackNode.parent != ROOT_PARENT) {
			nodes[stackNode.parent].rightOffset--;

			if (nodes[stackNode.parent].rightOffset == TOUCHED_TWICE)
				nodes[stackNode.parent].rightOffset = nodes.size() - 1 - stackNode.parent;
		}

		// If this is leaf no need to subdivide
		if (node.rightOffset == 0)
			continue;

		// get longest dimension, which we will split
		auto dim = bc.maxDimension();

		// split in center of longest axis
		float splitCoord = 0.5f * (bc.min()[dim] + bc.max()[dim]);

		// Partition the list of objects on this split
		size_t mid = start;
		for (size_t i = start; i < end; ++i) {
			if (first[i]->centroid()[dim] < splitCoord) {
				std::swap(first[i], first[mid]);
				++mid;
			}
		}

		// if we get bad split just choose center
		if (mid == start || mid == end)
			mid = start + (end - start) / 2;

		// push left child
		BuildEntry left = { nodes.size() - 1, mid, end };
		stack.push_back(left);

		// push right child
		BuildEntry right = { nodes.size() - 1, start, mid };
		stack.push_back(right);
	}

	BVH* result = new BVH;
	result->m_leafSize = leafSize;
	result->m_nodes = std::move(nodes);
	result->m_numLeafs = numLeafs;
	return result;
}

#endif // BVH_H