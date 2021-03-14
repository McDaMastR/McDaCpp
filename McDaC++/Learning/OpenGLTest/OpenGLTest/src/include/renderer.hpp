#pragma once

#include "vertexArray.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"

#include <cstdlib>

#ifdef DEBUG
	#define ASSERT(x) 	if (!(x)) {std::exit(2);}

	#define GL_CALL(x)	clearErrors();\
						x;\
						ASSERT(logCall(#x, __FILE__, __LINE__))
#else
	#define ASSERT(x)
	#define GL_CALL(x) x
#endif

void clearErrors();
bool logCall(const char *func, const char *file, const unsigned int line);

class Renderer
{
public:
	void clear() const;
	void setClearColor(const float v0, const float v1, const float v2, const float v3) const;
	void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
