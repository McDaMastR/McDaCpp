#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	GLuint m_ID;
	std::uint32_t m_count;

public:
    IndexBuffer(const std::uint32_t * const data, const std::uint32_t count);
    ~IndexBuffer();

    void   bind() const;
    void unBind() const;

    std::uint32_t getCount() const;
};
