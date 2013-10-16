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

/**
 * Manages shaders.
 * Shaders are accessed by name and are cached.
 * Currently shader source files are grouped together by file basename which
 * serves as program name e.g. files "simple.vert" and "simple.frag" form program "simple"
 */
class ShaderManager
{
public:
	ShaderManager() : m_storagePath(DEFAULT_STORAGE_PATH) { }
	/// ctor. You can pass your own directory where shaders are stored
	explicit ShaderManager(std::string shadersDir) : m_storagePath(std::move(shadersDir)) { }

	/**
	 * Gets GLSL shader program by name.
	 * Shader programs are cached, so actual compiling and linking is called
	 * only when program is returned for first time
	 * @param name program name
	 * @returns linked shader program or nullptr when loading failed.
	 */
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
