#include "include/shader.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
    : m_vertexFilePath(vertex_path), m_fragmentFilePath(fragment_path)
{
    auto[vertex_shader, fragment_shader] = parseShader(m_vertexFilePath, m_fragmentFilePath);
    createShader(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_rendererID));
}

void Shader::bind() const
{
    GLCALL(glUseProgram(m_rendererID));
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

unsigned int Shader::getUniformLocation(const std::string &name) const
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
	GLCALL(int location = glGetUniformLocation(m_rendererID, name.c_str()));
	if (location == -1)
        std::cout << "Warning: Uniform " << name << " does not exist\n";
    m_uniformLocationCache[name] = location;
    return location;
}

std::pair<std::string, std::string> Shader::parseShader(const std::string &vertex_shader, const std::string &fragment_shader) const
{
	std::ifstream file[2];
	std::stringstream ss[2];

	file[0].open(vertex_shader);
	ss[0] << file[0].rdbuf();

	file[1].open(fragment_shader);
	ss[1] << file[1].rdbuf();

	return {ss[0].str(), ss[1].str()};
}

void Shader::createShader(const std::string &vertex_shader, const std::string &fragment_shader)
{
	GLCALL(m_rendererID = glCreateProgram());
	const uint32_t vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
	const uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

	GLCALL(glAttachShader(m_rendererID, vs));
	GLCALL(glAttachShader(m_rendererID, fs));
	GLCALL(glLinkProgram(m_rendererID));
	GLCALL(glValidateProgram(m_rendererID));

	GLCALL(glDetachShader(m_rendererID, vs));
	GLCALL(glDetachShader(m_rendererID, fs));
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	int32_t debug_result;
	glGetProgramiv(m_rendererID, GL_LINK_STATUS, &debug_result);
	if (!debug_result) {
		int32_t length;
		GLCALL(glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &length));
		char *error_msg = new char[length];
		GLCALL(glGetProgramInfoLog(m_rendererID, length, nullptr, error_msg));

		std::cout << "Shader Linking Error: " << error_msg << '\n';;
		GLCALL(glDeleteProgram(m_rendererID));
		delete[] error_msg;
	}
}

unsigned int Shader::compileShader(const uint32_t type, const std::string &source) const
{
	GLCALL(const uint32_t id = glCreateShader(type));
	const char *src = source.c_str();
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	int32_t debug_result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &debug_result));
	if (!debug_result) {
		int32_t length;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *error_msg = new char[length];
		GLCALL(glGetShaderInfoLog(id, length, nullptr, error_msg));

		std::cout << "Shader Compilation Error: " << error_msg << '\n';
		GLCALL(glDeleteShader(id));
		delete[] error_msg;
		return 0;
	}

	return id;
}
