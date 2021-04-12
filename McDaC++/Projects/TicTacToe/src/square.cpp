#include "include/square.hpp"

#include <iostream>

Square::Square()
	: m_indices({0, 1, 2, 2, 3, 0}), m_ibo(m_indices.data(), 6), 
	m_vbo(m_vertices.data(), 8 * sizeof(float))
{
	m_vao.bind();
	m_vbo.bind();
	
	m_layout.push<float>(2);
	m_vao.addBuffer(m_layout);	
}

void Square::render(const Renderer &renderer) const
{
	m_vao.bind();
	m_ibo.bind();
	renderer.draw(m_ibo.getCount());
}

void Square::changeVertices(const std::uint8_t index)
{
	auto getPos = [=](const bool x_axis, const bool right) -> float {
		return (x_axis * (index % 3) + (!x_axis) * (index / 3)) * 2.0f / 3.0f - 1.0f + right * 2.0f / 3.0f;
	};

	m_vertices = {
		getPos(true, false), getPos(false, false),
		getPos(true, true ), getPos(false, false),
		getPos(true, true ), getPos(false, true ),
		getPos(true, false), getPos(false, true )
	};
}
