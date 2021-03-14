#include "include/renderer.hpp"

#include <GL/glew.h>
#include <iostream>

void clearErrors()
{
	while (glGetError());
}

bool logCall(const char *func, const char *file, const unsigned int line)
{
	unsigned int error;
	bool isError = false;
	while ((error = glGetError()))
	{
		std::cout << "OpenGL Error has occured\nError code: 0x" << std::hex << error << 
		std::dec << "\nFile: " << file << "\nLine: " << line << "\nFunction: " << func << '\n';
		isError = true;
	}
	if (isError)
		return false;
	return true;
}

void Renderer::clear() const
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}
	
void Renderer::setClearColor(const float r, const float g, const float b, const float a) const
{
	GL_CALL(glClearColor(r, g, b, a));
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
	shader.bind();
	va.bind();
	ib.bind();
	GL_CALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
