/**
 * @file ShaderManager.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "Shader.h"

#include <memory>
#include <map>
#include <string>

namespace gl {
	class ShaderProgram;
}

class ShaderManager
{
public:
	ShaderManager() : m_storagePath(DEFAULT_STORAGE_PATH) { }
	explicit ShaderManager(std::string shadersDir) : m_storagePath(std::move(shadersDir)) { }

	std::shared_ptr<gl::ShaderProgram> getGlslProgram(const std::string& name);
private:
	static const char* DEFAULT_STORAGE_PATH;

	std::shared_ptr<gl::ShaderProgram> loadProgram(const std::string& name);
	std::shared_ptr<gl::Shader> loadShader(const std::string& name, gl::Shader::Type type);
	static std::string shaderTypeFileSuffix(gl::Shader::Type type);

	std::map<std::string, std::shared_ptr<gl::ShaderProgram>> m_programs;
	std::string m_storagePath;
};

#endif // !SHADER_MANAGER_H
