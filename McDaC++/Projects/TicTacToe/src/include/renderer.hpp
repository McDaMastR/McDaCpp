#pragma once

#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"

#include <cstdlib>

#ifdef DEBUG
    #define ASSERT(x) if (!(x)) {std::exit(-2);}
    #define GLCALL(x)  clearErrors();\
                        x;\
                        ASSERT(logCall(#x, __FILE__, __LINE__));
#else
    #define ASSERT(x)
    #define GLCALL(x) x
#endif

#define BIT_SHIFT(x) 1 << x;

void clearErrors();
std::string errorType(const uint32_t type);
bool logCall(const char * const func, const char * const file, const uint32_t line);

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

	void newFrame() const;
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
