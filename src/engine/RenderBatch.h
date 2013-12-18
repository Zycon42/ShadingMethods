/**
 * @file RenderBatch.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef RENDER_BATCH_H
#define RENDER_BATCH_H

#include "Common.h"
#include "Buffer.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"

#include <vector>

namespace gl {

/**
 * OpenGL geometry stuff.
 * vao, vbo, ebo, draw mode
 */
class GeometryBatch
{
public:
	GeometryBatch() : m_vertexCount(0), m_indexCount(0), m_primitiveTypeSet(false) {
	}

	VertexArrayObject& vao() {
		return m_vao;
	}

	bool isReadyToDraw() {
		return m_vbo && m_primitiveTypeSet;
	}

	bool hasElements() {
		return static_cast<bool>(m_ebo);
	}

	void setPrimitiveType(PrimitiveType type);

	void setVertices(const std::shared_ptr<Buffer>& vbo, size_t count, const std::vector<VertexElement>& layout);

	void setIndices(const std::shared_ptr<Buffer>& ebo, GLenum type, size_t count);

	GLenum drawMode() const {
		return m_drawMode;
	}

	GLenum elementsType() const {
		return m_elementsType;
	}

	Buffer* vbo() {
		return m_vbo.get();
	}

	Buffer* ebo() {
		return m_ebo.get();
	}

	size_t indexCount() const {
		return m_indexCount;
	}

	size_t vertexCount() const {
		return m_vertexCount;
	}
private:
	GeometryBatch(GeometryBatch&);
	GeometryBatch& operator=(const GeometryBatch&);

	VertexArrayObject m_vao;
	std::shared_ptr<Buffer> m_vbo;
	size_t m_vertexCount;
	std::shared_ptr<Buffer> m_ebo;
	size_t m_indexCount;
	GLenum m_elementsType;
	GLenum m_drawMode;
	bool m_primitiveTypeSet;
};

/**
 * All things necessary for draw call.
 */
class RenderBatch
{
public:
	// movable p.s. in msvc11 there's no (= default/delete) so we have to use this stupid way
	RenderBatch() : shader(nullptr), materialUbo(nullptr), nodeUbo(nullptr), geometry(nullptr) { }
	RenderBatch(RenderBatch&& other) 
		: shader(other.shader), materialUbo(other.materialUbo), nodeUbo(other.nodeUbo), geometry(std::move(other.geometry))
	{ }

	RenderBatch& operator=(RenderBatch&& other) {
		this->shader = other.shader;
		this->materialUbo = other.materialUbo;
		this->nodeUbo = other.nodeUbo;
		this->geometry = std::move(other.geometry);
		return *this;
	}

	/// Shader we will use
	gl::ShaderProgram* shader;
	/// UBO for material
	gl::IndexedBuffer* materialUbo;
	/// UBO for node transforms
	gl::IndexedBuffer* nodeUbo;
	/// Geometry stuff
	std::unique_ptr<GeometryBatch> geometry;
private:
	// noncopyable
	RenderBatch(RenderBatch&);
	RenderBatch& operator=(const RenderBatch&);
};

}

#endif // !RENDER_BATCH_H
