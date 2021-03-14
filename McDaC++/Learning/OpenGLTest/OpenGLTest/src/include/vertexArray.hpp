#pragma once

#include "vertexBuffer.hpp"

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int rendererID;

public:
    VertexArray();
    ~VertexArray();

    void   bind() const;
    void unBind() const;

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
};