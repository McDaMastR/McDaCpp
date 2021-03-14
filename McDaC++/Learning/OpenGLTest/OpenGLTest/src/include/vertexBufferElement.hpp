#pragma once

struct VertexBufferElement
{
    unsigned int type, count;
    unsigned short normalized;

    static unsigned int typeSize(const unsigned int type);
};