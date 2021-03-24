#pragma once

#include <cstdint>

struct VertexBufferElement
{
    uint32_t m_type, m_count;
    uint16_t m_normalized;

    static uint32_t typeSize(const uint32_t type);
};
