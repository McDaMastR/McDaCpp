#include "include/vertexBuffer.hpp"
#include "include/renderer.hpp"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void * const data, const unsigned int size)
{
	GL_CALL(glGenBuffers(1, &rendererID));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID)); 
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID)); 
}

void VertexBuffer::unBind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
}
