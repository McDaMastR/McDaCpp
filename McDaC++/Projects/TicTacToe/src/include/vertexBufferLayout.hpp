#pragma once

#include "vertexBufferElement.hpp"

#include <vector>

class VertexBufferLayout
{
private:
    uint32_t m_stride;
    std::vector<VertexBufferElement> m_elements;

public:
    VertexBufferLayout();
    ~VertexBufferLayout() = default;

    unsigned int getStride() const;
    const std::vector<VertexBufferElement> &getElements() const;

    template <class T>
    void push(const uint32_t);
};
