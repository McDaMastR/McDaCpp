#pragma once

class IndexBuffer
{
private:
    unsigned int m_rendererID;
    unsigned int m_count;

public:
    IndexBuffer(const unsigned int * const data, const unsigned int count);
    ~IndexBuffer();

    void   bind() const;
    void unBind() const;

    unsigned int getCount() const;
};
