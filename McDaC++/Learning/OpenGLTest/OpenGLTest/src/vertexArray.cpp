#include "include/vertexArray.hpp"
#include "include/renderer.hpp"
#include "include/vertexBufferLayout.hpp"

#include <GL/glew.h>

VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &rendererID));
	GL_CALL(glBindVertexArray(rendererID));
}

VertexArray::~VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::bind() const
{
    GL_CALL(glBindVertexArray(rendererID));
}

void VertexArray::unBind() const
{
    GL_CALL(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
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
