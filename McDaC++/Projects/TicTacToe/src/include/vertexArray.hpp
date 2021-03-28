#pragma once

#include "vertexBuffer.hpp"

class VertexBufferLayout;

class VertexArray
{
private:
    uint32_t m_ID;

public:
    VertexArray();
    ~VertexArray();

    void   bind() const;
    void unBind() const;

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
};
