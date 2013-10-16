/**
 * @file ShaderProgram.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "ShaderProgram.h"

#include "Shader.h"
#include "Common.h"

namespace gl {

ShaderProgram::ShaderProgram() : m_isLinked(false) {
	m_handle = glCreateProgram();
}

void ShaderProgram::dispose() {
	glDeleteProgram(m_handle);
}

void ShaderProgram::link() {
	glLinkProgram(m_handle);

	// test if link successful
	int successful;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &successful);
	if (successful == GL_FALSE) {
		int buffLen;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &buffLen);
		std::unique_ptr<char[]> buff(new char[buffLen]);
		glGetProgramInfoLog(m_handle, buffLen, nullptr, buff.get());

		throw ShaderException(("Shader program linking failed. LOG: " + std::string(buff.get())).c_str());
	}

	m_isLinked = true;
}

void ShaderProgram::attachShader(const std::shared_ptr<Shader>& shader) {
	m_shaders.push_back(shader);
	glAttachShader(m_handle, shader->handle());
}

void ShaderProgram::attachShaders(const std::vector<std::shared_ptr<Shader>>& shaders) {
	for (auto shader : shaders) {
		attachShader(shader);
	}
}

void ShaderProgram::detachShader(const std::shared_ptr<Shader>& shader) {
	auto foundIt = std::find(m_shaders.begin(), m_shaders.end(), shader);
	if (foundIt != m_shaders.end()) {
		glDetachShader(m_handle, shader->handle());
		m_shaders.erase(foundIt);
	}
}

class SetShaderUniformVisitor : public boost::static_visitor<>
{
public:
	SetShaderUniformVisitor(gl::ShaderProgram* shader, int location)
		: m_shader(shader), m_location(location) { }

	void operator()(const int& value) const {
		glProgramUniform1i(m_shader->handle(), m_location, value);
	}

	void operator()(const unsigned int& value) const {
		glProgramUniform1ui(m_shader->handle(), m_location, value);
	}

	void operator()(const float& value) const {
		glProgramUniform1f(m_shader->handle(), m_location, value);
	}

	void operator()(const glm::vec2& value) const {
		glProgramUniform2fv(m_shader->handle(), m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::vec3& value) const {
		glProgramUniform3fv(m_shader->handle(), m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::vec4& value) const {
		glProgramUniform4fv(m_shader->handle(), m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::ivec2& value) const {
		glProgramUniform2iv(m_shader->handle(), m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::ivec3& value) const {
		glProgramUniform3iv(m_shader->handle(), m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::ivec4& value) const {
		glProgramUniform4iv(m_shader->handle(), m_location, 1, glm::value_ptr(value));
	}

	void operator()(const glm::mat2& value) const {
		glProgramUniformMatrix2fv(m_shader->handle(), m_location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void operator()(const glm::mat3& value) const {
		glProgramUniformMatrix3fv(m_shader->handle(), m_location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void operator()(const glm::mat4& value) const {
		glProgramUniformMatrix4fv(m_shader->handle(), m_location, 1, GL_FALSE, glm::value_ptr(value));
	}
private:
	gl::ShaderProgram* m_shader;
	int m_location;
};

void ShaderProgram::setUniform(const char* name, const UniformValue& value) {
	GLuint loc = glGetUniformLocation(m_handle, name);
	boost::apply_visitor(SetShaderUniformVisitor(this, loc), const_cast<UniformValue&>(value));
}

}