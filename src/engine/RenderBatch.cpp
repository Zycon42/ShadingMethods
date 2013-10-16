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

void GeometryBatch::setVertices(const std::shared_ptr<Buffer>& vbo, size_t count, const std::vector<VertexElement>& layout) {
	m_vbo = vbo;
	m_vertexCount = count;
	m_vao.bind();

	vbo->bind(GL_ARRAY_BUFFER);
	for (size_t i = 0; i < layout.size(); ++i) {
		auto element = layout[i];

		glEnableVertexAttribArray(i);
		switch (element.type)
		{
		case VertexElementType::Float:
			glVertexAttribPointer(i, element.numComponents, GL_FLOAT, GL_FALSE, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		case VertexElementType::Double:
			glVertexAttribLPointer(i, element.numComponents, GL_DOUBLE, 
				element.stride, reinterpret_cast<const void*>(element.offset));
		case VertexElementType::Byte:
			glVertexAttribIPointer(i, element.numComponents, GL_BYTE, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		case VertexElementType::UnsignedByte:
			glVertexAttribIPointer(i, element.numComponents, GL_UNSIGNED_BYTE, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		case VertexElementType::Short:
			glVertexAttribIPointer(i, element.numComponents, GL_SHORT, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		case VertexElementType::UnsignedShort:
			glVertexAttribIPointer(i, element.numComponents, GL_UNSIGNED_SHORT, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		case VertexElementType::Int:
			glVertexAttribIPointer(i, element.numComponents, GL_INT, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		case VertexElementType::UsignedInt:
			glVertexAttribIPointer(i, element.numComponents, GL_UNSIGNED_INT, 
				element.stride, reinterpret_cast<const void*>(element.offset));
			break;
		}
	}
}

}
