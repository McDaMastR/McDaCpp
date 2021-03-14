#pragma once

struct VertexBufferElement
{
    unsigned int m_type, m_count;
    unsigned short m_normalized;

    constexpr static unsigned int typeSize(const unsigned int type);
};
