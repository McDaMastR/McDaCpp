#pragma once

#include <cstdint>

class IndexBuffer
{
private:
	uint32_t m_ID;
	uint32_t m_count;

public:
    IndexBuffer(const uint32_t * const data, const uint32_t count);
    ~IndexBuffer();

    void   bind() const;
    void unBind() const;

    uint32_t getCount() const;
};
