#version 460 core
#extension GL_ARB_separate_shader_objects : enable

// Get colors from vertex shader
layout(location = 0) in vec3 v_fragColor;
layout(location = 1) in vec2 v_texCoord;

// Create variable to output color
layout(location = 0) out vec4 o_color;

// Create a combined image sampler
layout(binding = 1) uniform sampler2D u_texSampler;

void main()
{
	// Set output color
	o_color = texture(u_texSampler, v_texCoord);
}