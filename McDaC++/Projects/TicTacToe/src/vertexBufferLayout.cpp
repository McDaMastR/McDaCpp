#include "include/vertexBufferLayout.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>

VertexBufferLayout::VertexBufferLayout()
    : m_stride(0) {}

std::uint32_t VertexBufferLayout::getStride() const
{
    return m_stride;
}

const std::vector<VertexBufferElement> &VertexBufferLayout::getElements() const
{
    return m_elements;
}

template <class T>
void VertexBufferLayout::push(const std::uint32_t)
{
    assert(false);
}

template <>
void VertexBufferLayout::push<float>(const std::uint32_t count)
{
    m_elements.emplace_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
    m_stride += VertexBufferElement::typeSize(GL_FLOAT) * count;
}
