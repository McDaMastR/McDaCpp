#pragma once

#include "vertexBufferElement.hpp"

#include <vector>

class VertexBufferLayout
{
private:
    std::uint32_t m_stride;
    std::vector<VertexBufferElement> m_elements;

public:
    VertexBufferLayout();
    ~VertexBufferLayout() = default;

    std::uint32_t getStride() const;
    const std::vector<VertexBufferElement> &getElements() const;

    template <class T>
    void push(const std::uint32_t);
};
