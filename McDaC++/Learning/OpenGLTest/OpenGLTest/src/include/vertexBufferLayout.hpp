#pragma once

#include <vector>

#include "vertexBufferElement.hpp"

class VertexBufferLayout
{
private:
    unsigned int stride = 0;
    std::vector<VertexBufferElement> elements;

public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    unsigned int getStride() const;
    const std::vector<VertexBufferElement> &getElements() const;

    template <class T>
    void push(const unsigned int);
};