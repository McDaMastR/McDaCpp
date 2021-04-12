#pragma once

#include "GLclasses.hpp"

#include <array>

class Square
{
private:
	std::array<float, 8> m_vertices;
	const std::array<std::uint32_t, 6> m_indices;
	const IndexBuffer m_ibo;

	const VertexBuffer m_vbo;
	VertexBufferLayout m_layout;
	VertexArray m_vao;
    
public:
	Square();
	~Square() = default;

	void render(const Renderer &renderer) const;

	void changeVertices(const std::uint8_t index);
};
