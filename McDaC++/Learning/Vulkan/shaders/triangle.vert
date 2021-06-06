#version 450
#extension GL_ARB_separate_shader_objects : enable

// Give colors to fragment shader
layout(location = 0) out vec3 fragColor;

// Hard-coding positions because vertex buffer is too complicated for now
vec2 positions[3] = vec2[]
(
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
);

// Colors for fragment shader
vec3 colors[3] = vec3[]
(
	vec3(1.0, 0.0, 0.0), // Vertex 0: Red
	vec3(0.0, 1.0, 0.0), // Vertex 1: Green
	vec3(0.0, 0.0, 1.0)  // Vertex 2: Blue
);

void main()
{
	// Set vertex position
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);

	// Set fragment color
	fragColor = colors[gl_VertexIndex];
}