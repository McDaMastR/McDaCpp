#include "include/vertexBuffer.hpp"
#include "include/renderer.hpp"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void * const data, const unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_rendererID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::unBind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
