#pragma once

#include "vertexBuffer.hpp"
#include "vertexBufferLayout.hpp"

class VertexArray
{
private:
    GLuint m_ID;
	// std::vector<VertexBuffer *> buffers;

public:
    VertexArray();
    ~VertexArray();

    void   bind() const;
    void unBind() const;

    void addBuffer(const VertexBufferLayout &layout);
};
