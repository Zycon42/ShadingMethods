/**
 * @file Mesh.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Mesh.h"

void Mesh::loadVertices(std::vector<char> data, size_t count, std::vector<VertexElement> layout) {
	m_vertexData = std::move(data);
	m_vertexCount = count;
	m_vertexLayout = std::move(layout);
}

void Mesh::loadVertices(ArrayRef<char> data, size_t count, ArrayRef<VertexElement> layout) {
	assert(data.data() != nullptr && data.size() > 0 && layout.data() != nullptr && layout.size() > 0);

	m_vertexData.assign(data.data(), data.data() + data.size());
	m_vertexLayout.assign(layout.data(), layout.data() + layout.size());
	m_vertexCount = count;
}

void Mesh::loadIndices(ArrayRef<uint32_t> indices) {
	assert(indices.data() != nullptr && indices.size() > 0);
	m_indices.assign(indices.data(), indices.data() + indices.size());
}

Mesh* Mesh::create(std::vector<char> vertexData, size_t vertexCount, 
				   std::vector<VertexElement> vertexLayout, PrimitiveType primitiveType) {

	auto mesh = new Mesh();
	mesh->setPrimitiveType(primitiveType);
	mesh->loadVertices(std::move(vertexData), vertexCount, std::move(vertexLayout));
	return mesh;
}

Mesh* Mesh::create(std::vector<char> vertexData, size_t vertexCount, 
				   std::vector<VertexElement> vertexLayout, std::vector<uint32_t> indices, PrimitiveType primitiveType) {

	auto mesh = new Mesh();
	mesh->setPrimitiveType(primitiveType);
	mesh->loadVertices(std::move(vertexData), vertexCount, std::move(vertexLayout));
	mesh->loadIndices(std::move(indices));
	return mesh;
}
