/**
 * @file ShaderManager.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "ShaderManager.h"

#include "ShaderProgram.h"
#include "Helpers.h"
#include "Common.h"
#include "Logging.h"

const char* ShaderManager::DEFAULT_STORAGE_PATH = "../data/shaders/";

std::shared_ptr<gl::ShaderProgram> ShaderManager::getGlslProgram(const std::string& name) {
	auto it = m_programs.find(name);
	if (it != m_programs.end())
		return it->second;

	// shader program not yet loaded
	try {
		m_programs[name] = loadProgram(name);
	} catch (ShaderException& e) {
		LOG(ERROR) << "Unable to load program: " << e.what();
		return nullptr;
	}
	LOG(INFO) << "Successfully loaded program [" << name << "]";
	return m_programs[name];
}

std::shared_ptr<gl::ShaderProgram> ShaderManager::loadProgram(const std::string& name) {
	std::vector<std::shared_ptr<gl::Shader>> shaders;
	static gl::Shader::Type types[] = { 
		gl::Shader::Type::Vertex, gl::Shader::Type::Fragment, gl::Shader::Type::Geometry,
		gl::Shader::Type::TessControl, gl::Shader::Type::TessEvaluation
	};

	for (int i = 0; i < (sizeof(types) / sizeof(*types)); ++i) {
		auto shader = loadShader(name, types[i]);
		if (shader) {
			shaders.push_back(std::move(shader));
		}
	}

	auto prog = std::make_shared<gl::ShaderProgram>();
	prog->attachShaders(shaders);
	prog->link();
	return prog;
}

std::string ShaderManager::shaderTypeFileSuffix(gl::Shader::Type type) {
	static std::map<gl::Shader::Type, std::string> typeSuffixes = create_map<gl::Shader::Type, std::string>
		(gl::Shader::Type::Vertex, "vert")(gl::Shader::Type::Fragment, "frag")
		(gl::Shader::Type::Geometry, "geom")(gl::Shader::Type::TessControl, "tesc")
		(gl::Shader::Type::TessEvaluation, "tese");

	return typeSuffixes[type];
}

std::shared_ptr<gl::Shader> ShaderManager::loadShader(const std::string& name, gl::Shader::Type type) {
	auto shader = std::shared_ptr<gl::Shader>(
		gl::Shader::loadFromFile((m_storagePath + name + "." + shaderTypeFileSuffix(type)).c_str(), type)
	);

	if (!shader)
		return nullptr;

	shader->compile();

	return shader;
}
