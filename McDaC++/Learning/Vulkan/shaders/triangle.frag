#version 460 core
#extension GL_ARB_separate_shader_objects : enable

// Get colors from vertex shader
layout(location = 0) in vec3 v_fragColor;

// Create variable to output color
layout(location = 0) out vec4 o_color;

void main()
{
	// Set output color
	o_color = vec4(v_fragColor, 1.0);
}