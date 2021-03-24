#include "include/vertexArray.hpp"

#include "include/renderer.hpp"
#include "include/vertexBufferLayout.hpp"

#include <GL/glew.h>

VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &m_rendererID));
    GLCALL(glBindVertexArray(m_rendererID));
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::bind() const
{
    GLCALL(glBindVertexArray(m_rendererID));
}

void VertexArray::unBind() const
{
    GLCALL(glBindVertexArray(m_rendererID));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bind();
    const std::vector<VertexBufferElement> &elements = layout.getElements();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i, element.m_count, element.m_type, element.m_normalized, layout.getStride(), reinterpret_cast<const void *>(offset)));
        offset += element.m_count * VertexBufferElement::typeSize(element.m_type);
    }
}
