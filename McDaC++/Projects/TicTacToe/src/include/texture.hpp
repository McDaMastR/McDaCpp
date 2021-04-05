#pragma once

#include "renderer.hpp"
#include "GLclasses.hpp"

#include <array>

class Texture
{
private:
	const std::array<float, 16> m_vertices;

	const std::array<std::uint32_t, 6> m_indices;
	const IndexBuffer m_ibo;

	VertexArray m_vao;
	const VertexBuffer m_vbo;
	VertexBufferLayout m_layout;

   	GLuint m_ID;
    std::string m_filePath;
    unsigned char *m_localBuffer;
    std::int32_t m_width, m_height, m_BPP;

public:
    Texture(const std::string &file_path, const std::array<float, 16> &vertices, const std::array<std::uint32_t, 6> &indices);
    Texture(const std::string &file_path, const std::uint8_t index, const std::array<std::uint32_t, 6> &indices);
    virtual ~Texture();

	void render(const Renderer &renderer) const;

    std::int32_t getWidth () const;
    std::int32_t getHieght() const;
    std::int32_t getBPP   () const;

    void   bind(const std::uint32_t slot = 0) const;
    void unBind() const;

	static std::array<float, 16> indexToVertex(std::uint16_t index);
};
