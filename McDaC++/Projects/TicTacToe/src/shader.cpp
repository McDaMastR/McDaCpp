#include "include/shader.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &file_path)
    : m_filePath(file_path)
{
    auto[vertex_shader, fragment_shader] = parseShader(m_filePath);
    m_rendererID = createShader(vertex_shader, fragment_shader);
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

void Shader::setUniformVeci(const std::string &name, const int v0)
{
    GLCALL(glUniform1i(getUniformLocation(name), v0));
}

void Shader::setUniformVecf(const std::string &name, const glm::vec4 &values)
{
    GLCALL(glUniform4f(getUniformLocation(name), values.x, values.y, values.z, values.w));
}

void Shader::setUniformMatf(const std::string &name, const glm::mat4 &matrix)
{
    GLCALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
