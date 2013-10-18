/**
 * @file RenderBatch.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "RenderBatch.h"

namespace gl {

GLuint VertexArrayObject::s_bindVao = 0;

void GeometryBatch::setPrimitiveType(PrimitiveType type) {
	switch (type)
	{
	case PrimitiveType::Points:
		m_drawMode = GL_POINTS;
		break;
	case PrimitiveType::LineList:
		m_drawMode = GL_LINES;
		break;
	case PrimitiveType::LineStrip:
		m_drawMode = GL_LINE_STRIP;
		break;
	case PrimitiveType::TriangleList:
		m_drawMode = GL_TRIANGLES;
		break;
	case PrimitiveType::TriangleStrip:
		m_drawMode = GL_TRIANGLE_STRIP;
		break;
	default:
		return;
	}
	m_primitiveTypeSet = true;
}

void GeometryBatch::setIndices(const std::shared_ptr<Buffer>& ebo, GLenum type, size_t count) {
	m_ebo = ebo;
	m_elementsType = type;
	m_indexCount = count;

	m_vao.bind();
	ebo->bind(GL_ELEMENT_ARRAY_BUFFER);
}

static size_t elementTypeSize(VertexElementType type) {
	static size_t typeSizes[] = { 1, 1, 2, 2, 4, 4, 4, 8 };
	return typeSizes[static_cast<int>(type)];
}

static size_t computeStride(const std::vector<VertexElement>& layout) {
	size_t stride = 0;
	for (const auto& element : layout) {
		stride += element.numComponents * elementTypeSize(element.type);
	}
	return stride;
}

void GeometryBatch::setVertices(const std::shared_ptr<Buffer>& vbo, size_t count, const std::vector<VertexElement>& layout) {
	m_vbo = vbo;
	m_vertexCount = count;
	m_vao.bind();

	size_t computedStride = computeStride(layout);

	size_t previousOffset = 0;

	vbo->bind(GL_ARRAY_BUFFER);
	for (size_t i = 0; i < layout.size(); ++i) {
		auto element = layout[i];

		size_t offset = element.offset;
		// if offset is 0 then compute it
		if (i != 0 && offset == 0)
			offset = previousOffset + layout[i - 1].numComponents * elementTypeSize(layout[i - 1].type);

		previousOffset = offset;

		size_t stride = element.stride;
		if (stride == 0)
			stride = computedStride;

		glEnableVertexAttribArray(i);
		switch (element.type)
		{
		case VertexElementType::Float:
			glVertexAttribPointer(i, element.numComponents, GL_FLOAT, GL_FALSE, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		case VertexElementType::Double:
			glVertexAttribLPointer(i, element.numComponents, GL_DOUBLE, 
				stride, reinterpret_cast<const void*>(offset));
		case VertexElementType::Byte:
			glVertexAttribIPointer(i, element.numComponents, GL_BYTE, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		case VertexElementType::UnsignedByte:
			glVertexAttribIPointer(i, element.numComponents, GL_UNSIGNED_BYTE, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		case VertexElementType::Short:
			glVertexAttribIPointer(i, element.numComponents, GL_SHORT, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		case VertexElementType::UnsignedShort:
			glVertexAttribIPointer(i, element.numComponents, GL_UNSIGNED_SHORT, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		case VertexElementType::Int:
			glVertexAttribIPointer(i, element.numComponents, GL_INT, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		case VertexElementType::UsignedInt:
			glVertexAttribIPointer(i, element.numComponents, GL_UNSIGNED_INT, 
				stride, reinterpret_cast<const void*>(offset));
			break;
		}
	}
}

}
