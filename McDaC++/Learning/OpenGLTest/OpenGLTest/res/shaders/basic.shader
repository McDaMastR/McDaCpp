#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

out vec2 v_tex_coord;

uniform mat4 u_MVP; // Modle view projection matrix

void main()
{
	gl_Position = u_MVP * position;
	v_tex_coord = tex_coord;
}

#shader fragment
#version 330 core

in vec2 v_tex_coord;

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 tex_color = texture(u_Texture, v_tex_coord);
	color = tex_color;
}
