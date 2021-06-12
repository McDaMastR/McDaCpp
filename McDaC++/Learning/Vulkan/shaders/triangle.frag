#version 460 core
#extension GL_ARB_separate_shader_objects : enable

// Get colors from vertex shader
layout(location = 0) in vec3 fragColor;

// Create variable to output color
layout(location = 0) out vec4 outColor;

void main()
{
	// Set output color
	outColor = vec4(fragColor, 1.0);
}