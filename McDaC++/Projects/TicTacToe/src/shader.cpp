#include "include/shader.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &file_path)
    : m_shaders({{ShaderType::vertex, parseShader(file_path + ".vert")}, 
				{ShaderType::fragment, parseShader(file_path + ".frag")}})
{
    createProgram(m_shaders[ShaderType::vertex], m_shaders[ShaderType::fragment]);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_programID));
}

void Shader::bind() const
{
    GLCALL(glUseProgram(m_programID));
}

void Shader::unBind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::setUniformVeci(const std::string &name, const int32_t v0) const
{
    GLCALL(glUniform1i(getUniformLocation(name), v0));
}

void Shader::setUniformVecf(const std::string &name, const glm::vec4 &values) const
{
    GLCALL(glUniform4f(getUniformLocation(name), values.x, values.y, values.z, values.w));
}

void Shader::setUniformMatf(const std::string &name, const glm::mat4 &matrix) const
{
    GLCALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

uint32_t Shader::getUniformLocation(const std::string &name) const
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
	GLCALL(int32_t location = glGetUniformLocation(m_programID, name.c_str()));
	if (location == -1)
        std::cerr << "Warning: Uniform " << name << " does not exist\n";
    m_uniformLocationCache[name] = location;
    return location;
}

std::string Shader::parseShader(const std::string &file_path) const
{
	std::ifstream file(file_path);
	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

void Shader::createProgram(const std::string &vertex_shader, const std::string &fragment_shader)
{
	GLCALL(m_programID = glCreateProgram());
	const uint32_t vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
	const uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

	GLCALL(glAttachShader(m_programID, vs));
	GLCALL(glAttachShader(m_programID, fs));
	GLCALL(glLinkProgram(m_programID));

	GLCALL(glDetachShader(m_programID, vs));
	GLCALL(glDetachShader(m_programID, fs));
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	checkError(GL_LINK_STATUS, true);
	GLCALL(glValidateProgram(m_programID));
	checkError(GL_VALIDATE_STATUS, true);
}

uint32_t Shader::compileShader(const uint32_t type, const std::string &source_code) const
{
	GLCALL(const uint32_t shader_id = glCreateShader(type));
	if (!shader_id) {
		std::cerr << getShaderType(type) << " Creation Error\n";
		ASSERT(false);
	}

	const char * const src = source_code.c_str();
	GLCALL(glShaderSource(shader_id, 1, &src, nullptr));
	GLCALL(glCompileShader(shader_id));
	checkError(GL_COMPILE_STATUS, false, shader_id, type);

	return shader_id;
}
void Shader::checkError(const uint32_t flag, const bool is_program, const uint32_t shader_id, const uint32_t shader_type) const
{
	int32_t result;

	if (is_program) 
	{
		GLCALL(glGetProgramiv(m_programID, flag, &result));
		if (!result) {
			int32_t length;
			GLCALL(glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length));
			char * const error_msg = new char[length];
			GLCALL(glGetProgramInfoLog(m_programID, length, nullptr, error_msg));
			std::cerr << "Program Error: " << error_msg << '\n';

			GLCALL(glDeleteProgram(m_programID));
			delete[] error_msg;
			ASSERT(false);
		}
	}

	else 
	{
		GLCALL(glGetShaderiv(shader_id, flag, &result));
		if (!result) {
			int32_t length;
			GLCALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
			char * const error_msg = new char[length];
			GLCALL(glGetShaderInfoLog(shader_id, length, nullptr, error_msg));
			std::cerr << getShaderType(shader_type) << " Error: " << error_msg << '\n';

			GLCALL(glDeleteShader(shader_id));
			delete[] error_msg;
			ASSERT(false);
		}
	}
}
