#include "include/blender.hpp"

#include "include/renderer.hpp"

#include <GL/glew.h>

Blender::Blender(const std::uint32_t v0, const std::uint32_t v1)
{
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(v0, v1));
}
