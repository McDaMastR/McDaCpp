#version 460 core
#extension GL_ARB_separate_shader_objects : enable

// Attributes get information from vertex buffers
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;

// Give colors to fragment shader
layout(location = 0) out vec3 v_fragColor;

// Access the resource discriptor via uniform buffer Objects
layout(set = 0, binding = 0) uniform UniformBufferObject
{
	mat4 model;
	mat4 view;
	mat4 proj;
} u_UBO;

void main()
{
	// Set vertex position
	gl_Position = u_UBO.proj * u_UBO.view * u_UBO.model * vec4(a_position, 0.0, 1.0);

	// Set fragment color
	v_fragColor = a_color;
}