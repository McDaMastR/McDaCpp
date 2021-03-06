#include "include/vertexBufferElement.hpp"

#include "include/renderer.hpp"

std::uint32_t VertexBufferElement::typeSize(const GLuint type)
{
    switch (type)
    {
    case GL_FLOAT:
        return sizeof(float);
    case GL_DOUBLE:
        return sizeof(double);
    case GL_BYTE:
        return sizeof(char);
    case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
    case GL_SHORT:
        return sizeof(short);
    case GL_UNSIGNED_SHORT:
        return sizeof(unsigned short);
    case GL_INT:
        return sizeof(int);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
    default:
        assert(false);
    }
}
