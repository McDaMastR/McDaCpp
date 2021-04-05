#include "include/square.hpp"

Square::Square()
	: m_indices({0, 1, 2, 2, 3, 0}), m_ibo(m_indices.data(), 6), 
	m_vbo(m_vertices.data(), 8 * sizeof(float))
{
	m_vao.bind();
	m_ibo.bind();
	m_vbo.bind();
	
	m_layout.push<float>(2);
	m_vao.addBuffer(m_vbo, m_layout);	
}

void Square::render(const Renderer &renderer) const
{
	m_vao.bind();
	renderer.draw(m_ibo.getCount());
}

void Square::changeVertices(const std::uint8_t index)
{
	auto getPos = [=](const bool left) -> float {
		return (index % 3) * 2.0f / 3.0f + left * 2.0f / 3.0f - 1;
	};

	m_vertices = {
		getPos(true),  getPos(true),
		getPos(false), getPos(true),
		getPos(false), getPos(false),
		getPos(true),  getPos(false)
	};
}
