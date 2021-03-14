#include "include/indexBuffer.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int * const data, const unsigned int count)
    : m_count(count)
{
    GLCALL(glGenBuffers(1, &m_rendererID));  
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_rendererID));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::unBind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getCount() const
{
    return m_count;
}
