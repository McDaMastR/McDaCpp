#include "include/renderer.hpp"

#include <GL/glew.h>
#include <iostream>

void clearErrors()
{
    while (glGetError());
}

std::string errorType(const uint32_t type)
{
	switch (type)
	{
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		default:
			return "GL_NO_ERROR";
	}
}

std::string getShaderType(const uint32_t type)
{
	switch (type)
	{
		case GL_VERTEX_SHADER:
			return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER:
			return "GL_FRAGMENT_SHADER";
		default:
			return "UNKOWN SHADER TYPE";
	}
}

bool logCall(const char * const func, const char * const file, const uint32_t line)
{
   	uint32_t error;
	bool isError = false;
	while ((error = glGetError()))
	{
		std::cerr << "OpenGL Error has occured\nError code: 0x" << std::hex << error << 
		std::dec << "\nError type: " << errorType(error) << "\nFile: " << file << "\nLine: " << line << "\nFunction: " << func << '\n';
		isError = true;
	}
	if (isError)
		return false;
	return true; 
}

void Renderer::newFrame() const
{
    GLCALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.bind();
    va.bind();
    ib.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
