#pragma once

#include <cstdint>

class VertexBuffer
{
private:
    uint32_t m_rendererID;

public:
    VertexBuffer(const void * const data, const uint32_t size);
    ~VertexBuffer();

    void   bind() const;
    void unBind() const;
};
