#include "include/vertexBufferElement.hpp"
#include "include/renderer.hpp"

#include <GL/glew.h>

unsigned int VertexBufferElement::typeSize(const unsigned int type)
{
    switch (type)
    {
    case GL_FLOAT:
        return sizeof(float);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
    }
    ASSERT(false);
}
