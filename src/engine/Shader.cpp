/**
 * @file Shader.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Shader.h"

#include "Common.h"

#include <fstream>
#include <sstream>
#include <memory>

namespace gl {

Shader::Shader(Type type) : m_isCompiled(false) {
	m_handle = glCreateShader(static_cast<GLenum>(type));
}

Shader::Shader(std::string source, Type type) : m_source(std::move(source)), m_isCompiled(false) {
	m_handle = glCreateShader(static_cast<GLenum>(type));
}

void Shader::dispose() {
	glDeleteShader(m_handle);
}

Shader* Shader::loadFromFile(const char* fileName, Type type) {
	std::ifstream file(fileName);
	if (!file)
		return nullptr;

	std::ostringstream contents;
	contents << file.rdbuf();
	file.close();

	return new Shader(contents.str(), type);
}

void Shader::compile() {
	const char* source = m_source.c_str();
	glShaderSource(m_handle, 1, &source, nullptr);
	glCompileShader(m_handle);

	// test if compile successful
	int successful;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &successful);
	if (successful == GL_FALSE) {
		int buffLen;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &buffLen);
		std::unique_ptr<char[]> buff(new char[buffLen]);
		glGetShaderInfoLog(m_handle, buffLen, nullptr, buff.get());

		throw ShaderException(("Shader compilation failed. LOG: " + std::string(buff.get())).c_str());
	}

	m_isCompiled = true;
}

}
