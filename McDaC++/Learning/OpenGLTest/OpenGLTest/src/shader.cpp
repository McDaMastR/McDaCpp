#include "include/shader.hpp"
#include "include/renderer.hpp"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &path)
    : path(path)
{
	auto[vertex_shader, fragment_shader] = parseShader(path);
	rendererID = createShader(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    GL_CALL(glDeleteProgram(rendererID));
}

std::pair<std::string, std::string> Shader::parseShader(const std::string &file_path)
{
	enum class Mode
	{
		null = -1, vertex = 0, fragment = 1
	};

	std::ifstream file(file_path);
	std::string line;
	std::stringstream ss[2];
	Mode mode = Mode::null;

	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				mode = Mode::vertex;
			else if (line.find("fragment") != std::string::npos)
				mode = Mode::fragment;
			continue;
		}
		ss[static_cast<short>(mode)] << line << '\n';
	}
	return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::createShader(const std::string &vertex_shader, const std::string &fragment_shader)
{
	GL_CALL(unsigned int program = glCreateProgram());
	const unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
	const unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));
	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	GL_CALL(glDeleteShader(vs));
	GL_CALL(glDeleteShader(fs));
	return program;
}

void Shader::bind() const
{
    GL_CALL(glUseProgram(rendererID));
}

void Shader::unBind() const
{
    GL_CALL(glUseProgram(0));
}

void Shader::setUniformi(const std::string &name, const int v0)
{
    GL_CALL(glUniform1i(getUniformLocation(name), v0));
}

void Shader::setUniformf(const std::string &name, const float v0, const float v1, const float v2, const float v3)
{
	GL_CALL(glUniform4f(getUniformLocation(name), v0 ,v1, v2, v3));
}

void Shader::setUniformMatf(const std::string &name, const glm::mat4 &matrix)
{
    GL_CALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::getUniformLocation(const std::string &name) const
{
    if (uniform_location_cache.find(name) != uniform_location_cache.end())
        return uniform_location_cache[name];
	GL_CALL(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1)
        std::cout << "Warning: Uniform " << name << " does not exist\n";
    uniform_location_cache[name] = location;
    return location;
}

unsigned int Shader::compileShader(const unsigned int type, const std::string &source)
{
	GL_CALL(const unsigned int id = glCreateShader(type));
	const char *src = source.c_str();
	GL_CALL(glShaderSource(id, 1, &src, nullptr));
	GL_CALL(glCompileShader(id));

	int debug_result;
	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &debug_result));
	if (!debug_result)
	{
		int length;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *error_msg = static_cast<char *>(alloca(length * sizeof(char)));
		GL_CALL(glGetShaderInfoLog(id, length, &length, error_msg));

		std::cout << "Shader Error: " << error_msg << '\n';
		GL_CALL(glDeleteShader(id));
		return 0;
	}

	return id;
}
