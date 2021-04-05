#version 410 core

in vec2 v_tex_coord;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;

void main()
{
	vec4 tex_color = texture(u_Texture, v_tex_coord);
	color = tex_color;
}
