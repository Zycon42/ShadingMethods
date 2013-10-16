/**
 * @file Shader.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef SHADER_H
#define SHADER_H

#include <gl/glew.h>

#include <string>

namespace gl {

/**
 * Wrapper around OpenGL shader object.
 */
class Shader
{
public:
	enum class Type : GLenum { 
		Vertex = GL_VERTEX_SHADER, 
		Fragment = GL_FRAGMENT_SHADER, 
		Geometry = GL_GEOMETRY_SHADER, 
		TessControl = GL_TESS_CONTROL_SHADER, 
		TessEvaluation = GL_TESS_EVALUATION_SHADER
	};

	explicit Shader(Type type);
	Shader(std::string source, Type type);
	~Shader() { dispose(); }

	void dispose();

	static Shader* loadFromFile(const char* fileName, Type type);

	void compile();

	bool isCompiled() const {
		return m_isCompiled;
	}

	GLuint handle() const {
		return m_handle;
	}

	void setSource(std::string source) {
		m_source = std::move(source);
	}
private:
	Shader(const Shader&);
	Shader& operator=(Shader);

	std::string m_source;
	bool m_isCompiled;
	GLuint m_handle;
};

}

#endif // !SHADER_H
