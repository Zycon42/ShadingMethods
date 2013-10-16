/**
 * @file Mesh.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef MESH_H
#define MESH_H

#include "Common.h"

#include "ArrayRef.h"

#include <vector>
#include <cstdint>
#include <cassert>

/**
 * Geometry container.
 * Contains vertices and its layout, primitive type that they create
 * and optional indices.
 */
class Mesh
{
public:
	Mesh() : m_vertexCount(0), primType(PrimitiveType::TriangleList) { }

	static Mesh* create(std::vector<char> vertexData, size_t vertexCount, std::vector<VertexElement> vertexLayout,
		PrimitiveType primitiveType);

	static Mesh* create(std::vector<char> vertexData, size_t vertexCount, std::vector<VertexElement> vertexLayout,
		std::vector<uint32_t> indices, PrimitiveType primitiveType);

	/// Returns true if mesh contains any vertices and some format of them
	bool isValid() const {
		return m_vertexCount != 0 && !m_vertexLayout.empty();
	}

	/// Returns true if mesh contains any indices
	bool isIndexed() const {
		return !m_indices.empty();
	}

	PrimitiveType primitiveType() const {
		return primType;
	}

	void setPrimitiveType(PrimitiveType type) {
		primType = type;
	}

	void loadVertices(std::vector<char> data, size_t count, std::vector<VertexElement> layout);

	void loadVertices(ArrayRef<char> data, size_t count, ArrayRef<VertexElement> layout);

	void loadIndices(std::vector<uint32_t> indices) {
		m_indices = std::move(indices);
	}

	void loadIndices(ArrayRef<uint32_t> indices);

	std::vector<char>& vertexData() {
		return m_vertexData;
	}

	const std::vector<char>& vertexData() const {
		return m_vertexData;
	}

	size_t vertexCount() {
		return m_vertexCount;
	}

	std::vector<VertexElement>& vertexLayout() {
		return m_vertexLayout;
	}

	const std::vector<VertexElement>& vertexLayout() const {
		return m_vertexLayout;
	}

	std::vector<uint32_t>& indices() {
		return m_indices;
	}

	const std::vector<uint32_t>& indices() const {
		return m_indices;
	}

private:
	std::vector<char> m_vertexData;
	size_t m_vertexCount;
	std::vector<VertexElement> m_vertexLayout;
	PrimitiveType primType;
	std::vector<uint32_t> m_indices;
};

#endif // !MESH_H
