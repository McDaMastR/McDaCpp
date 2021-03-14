#include "include/vertexBufferLayout.hpp"

#include <GL/glew.h>

VertexBufferLayout::VertexBufferLayout()
{

}

VertexBufferLayout::~VertexBufferLayout()
{

}

unsigned int VertexBufferLayout::getStride() const
{
    return stride;
}

const std::vector<VertexBufferElement> &VertexBufferLayout::getElements() const
{
    return elements;
}

template <class T>
void VertexBufferLayout::push(const unsigned int)
{
    assert(false);
}

template <>
void VertexBufferLayout::push<float>(const unsigned int count)
{
    elements.emplace_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
    stride += VertexBufferElement::typeSize(GL_FLOAT) * count;
}

template <>
void VertexBufferLayout::push<unsigned int>(const unsigned int count)
{
    elements.emplace_back(VertexBufferElement{GL_UNSIGNED_INT, count, GL_FALSE});
    stride += VertexBufferElement::typeSize(GL_UNSIGNED_INT) * count;
}

template <>
void VertexBufferLayout::push<unsigned char>(const unsigned int count)
{
    elements.emplace_back(VertexBufferElement{GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += VertexBufferElement::typeSize(GL_UNSIGNED_BYTE) * count;
}
