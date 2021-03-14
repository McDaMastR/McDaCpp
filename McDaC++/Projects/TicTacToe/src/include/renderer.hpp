#pragma once

#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"

#include <cstdlib>

#ifdef DEBUG
    #define ASSERT(x) if (!(x)) {std::exit(-2);}
    #define GLCALL(x)  clearErrors();\
                        x;
                        ASSERT(logCall(#x, __FILE__, __LINE__));
#else
    #define ASSERT(x)
    #define GLCALL(x) x
#endif

void clearErrors();
bool logCall(const char * const func, const char * const file, const unsigned int line);

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

    void clear() const;
    void setClearColor(const float r, const float g, const float b, const float a) const;
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
