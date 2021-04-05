#pragma once

#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "vertexBufferLayout.hpp"
#include "shader.hpp"
#include "events.hpp"

#include <unordered_map>

#ifdef DEBUG
    #define GLCALL(x)  clearErrors();\
                        x;\
                        assert(logCall(#x, __FILE__, __LINE__));
#else
    #define GLCALL(x) x
#endif

#define BIT_SHIFT(x) 1 << x;

void clearErrors();
std::string errorType(const std::uint32_t type);
std::string getShaderType(const std::uint32_t type);
bool logCall(const char * const func, const char * const file, const std::uint32_t line);

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

	void newFrame() const;
    void draw(const std::uint32_t count) const;
};
