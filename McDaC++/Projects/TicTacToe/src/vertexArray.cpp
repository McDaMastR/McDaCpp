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

VertexArray::bind() const
{
    GLCALL(glBindVertexArray(m_rendererID));
}

VertexArray::unBind() const
{
    GLCALL(glBindVertexArray(m_rendererID));
}

VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bind();
    const std::vector<VertexBufferElement> &elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), reinterpret_cast<const void *>(offset)));
        offset += element.count * VertexBufferElement::typeSize(element.type);
    }
}
