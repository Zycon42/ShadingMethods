/**
 * @file Query.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef QUERY_H
#define QUERY_H

#include <GL/glew.h>

#include <algorithm>

namespace gl {
	class Query;
}

void swap(gl::Query& first, gl::Query& second);

namespace gl {

class Query
{
public:
	Query() : m_handle(0) {
		glGenQueries(1, &m_handle);
	}

	~Query() {
		if (m_handle != 0)
			glDeleteQueries(1, &m_handle);
	}

	Query(Query&& other) : m_handle(0) {
		swap(*this, other);
	}

	friend void (::swap)(Query& first, Query& second);

	void begin(GLenum target) {
		glBeginQuery(target, m_handle);
	}

	void end(GLenum target) {
		glEndQuery(target);
	}

	bool isResultAvailable() {
		int result = GL_FALSE;
		glGetQueryObjectiv(m_handle, GL_QUERY_RESULT_AVAILABLE, &result);
		return result == GL_TRUE;
	}

	void getResult(GLint* params) {
		glGetQueryObjectiv(m_handle, GL_QUERY_RESULT, params);
	}

	void getResult(GLuint* params) {
		glGetQueryObjectuiv(m_handle, GL_QUERY_RESULT, params);
	}

	void getResult(GLint64* params) {
		glGetQueryObjecti64v(m_handle, GL_QUERY_RESULT, params);
	}

	void getResult(GLuint64* params) {
		glGetQueryObjectui64v(m_handle, GL_QUERY_RESULT, params);
	}
private:
	Query(const Query&);
	Query& operator=(const Query&);

	GLuint m_handle;
};

}

inline void swap(gl::Query& first, gl::Query& second) {
	using std::swap;

	swap(first.m_handle, second.m_handle);
}

#endif // QUERY_H
