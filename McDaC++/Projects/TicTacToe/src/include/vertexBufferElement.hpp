#pragma once

#include <GL/glew.h>
#include <cstdint>

struct VertexBufferElement
{
    GLuint m_type;
	std::uint32_t m_count;
    std::uint16_t m_normalized;

    static std::uint32_t typeSize(const GLuint type);
};
