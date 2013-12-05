/**
 * @file VertexArrayObject.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include <GL/glew.h>

namespace gl {

/// Wrapper around OpenGL vao. Movable noncopyable
class VertexArrayObject
{
public:
	VertexArrayObject() {
		glGenVertexArrays(1, &m_handle);
	}

	~VertexArrayObject() {
		glDeleteVertexArrays(1, &m_handle);
	}

	VertexArrayObject(VertexArrayObject&& other)
		: m_handle(other.m_handle) { }

	VertexArrayObject& operator=(VertexArrayObject&& other) {
		this->m_handle = other.m_handle;
		return *this;
	}

	GLuint handle() const {
		return m_handle;
	}

	/// bind vao
	void bind() {
		if (m_handle != s_bindVao) {
			glBindVertexArray(m_handle);
			s_bindVao = m_handle;
		}
	}

	/// Unbind currently bound vao
	static void unbind() {
		glBindVertexArray(0);
		s_bindVao = 0;
	}
private:
	VertexArrayObject(const VertexArrayObject&);
	VertexArrayObject& operator=(VertexArrayObject);

	static GLuint s_bindVao;	// to avoid unnecessary state changes, here we store current bound vao

	GLuint m_handle;			// OpenGL handle
};

}

#endif // !VERTEX_ARRAY_OBJECT_H
