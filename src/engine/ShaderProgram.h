/**
 * @file ShaderProgram.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/variant/variant.hpp>

#include <memory>
#include <vector>

namespace gl {

class Shader;

typedef boost::variant<
	int, unsigned int, float, glm::vec2, glm::vec3, glm::vec4,
	glm::ivec2, glm::ivec3, glm::ivec4, glm::mat2, glm::mat3, glm::mat4
> UniformValue;

/**
 * Wrapper around OpenGL shader program.
 */
class ShaderProgram
{
public:
	ShaderProgram();

	~ShaderProgram() { 
		dispose(); 
	}

	void dispose();

	/**
	 * Attach shader to this program.
	 * Its allowed to pass uncompiled shader, but shader must be compiled
	 * before you call link. There's unlimited number of attached shaders of same type
	 * so attaching some shader won't in any case detach any other shader.
	 * @param shader shared_ptr to Shader because shaders can be shared between programs
	 */
	void attachShader(const std::shared_ptr<Shader>& shader);

	/// Attaches multiple shaders in single call.
	void attachShaders(const std::vector<std::shared_ptr<Shader>>& shaders);

	/**
	 * Detaches shader from program.
	 * @param shader if shader is not attached, function will do nothing
	 */
	void detachShader(const std::shared_ptr<Shader>& shader);

	/// Gets all attached shaders to this program.
	std::vector<std::shared_ptr<Shader>>& attachedShaders() {
		return m_shaders;
	}

	/**
	 * Associates vertex attribute index with vertex shader named attribute.
	 * @param index vertex attribute index.
	 * @param name name of vertex attribute in vertex shader.
	 */
	void bindAttribLocation(GLuint index, const char* name) {
		glBindAttribLocation(m_handle, index, name);
	}

	/**
	 * Returns program uniform GLuint value.
	 * @param value uniform name string.
	 * @return glGetUniformLocation(program, value).
	 */        
	GLuint getUniformLocation(const char* value) {
		return glGetUniformLocation(m_handle, value);
	}

	GLint getAttribLocation(const char* name) {
		return glGetAttribLocation(m_handle, name);
	}

	void use() {
		glUseProgram(m_handle);
	}

	/**
	 * Links program using glLinkProgram.
	 * @throws ShaderException if link failed.
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glLinkProgram.xml
	 */
	void link();

	bool isLinked() const {
		return m_isLinked;
	}

	GLuint handle() const {
		return m_handle;
	}

	/// Sets program uniform
	void setUniform(const char* name, const UniformValue& value);
private:
	ShaderProgram(const ShaderProgram&);
	ShaderProgram& operator=(ShaderProgram);

	GLuint m_handle;
	bool m_isLinked;

	std::vector<std::shared_ptr<Shader>> m_shaders;
};

}

#endif // !SHADER_PROGRAM_H

