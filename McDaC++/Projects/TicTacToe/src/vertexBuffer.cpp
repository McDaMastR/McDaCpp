#include "include/vertexBuffer.hpp"
#include "include/renderer.hpp"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void * const data, const std::uint32_t size)
{
    GLCALL(glGenBuffers(1, &m_ID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_ID));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VertexBuffer::unBind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
