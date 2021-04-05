#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
    GLuint m_ID;

public:
    VertexBuffer(const void * const data, const std::uint32_t size);
    ~VertexBuffer();

    void   bind() const;
    void unBind() const;
};
