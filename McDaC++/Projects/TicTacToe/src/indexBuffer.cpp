#include "include/indexBuffer.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(const uint32_t * const data, const uint32_t count)
    : m_count(count)
{
    GLCALL(glGenBuffers(1, &m_ID));  
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_ID));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::unBind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

uint32_t IndexBuffer::getCount() const
{
    return m_count;
}
