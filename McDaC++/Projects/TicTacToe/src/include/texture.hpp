#pragma once

#include "renderer.hpp"
#include "vertexArray.hpp"
#include "vertexBuffer.hpp"
#include "vertexBufferLayout.hpp"
#include "indexBuffer.hpp"

#include <array>

class Texture
{
private:
	const std::array<float, 16> m_vertices;
	const std::array<uint32_t, 6> m_indices;

	VertexArray m_vao;
	const VertexBuffer m_vbo;
	VertexBufferLayout m_layout;
	const IndexBuffer m_ibo;

    uint32_t m_ID;
    std::string m_filePath;
    unsigned char *m_localBuffer;
    int32_t m_width, m_height, m_BPP;

public:
    Texture(const std::string &file_path, const std::array<float, 16> &vertices, const std::array<uint32_t, 6> &indices);
    Texture(const std::string &file_path, const uint16_t index, const std::array<uint32_t, 6> &indices);
    virtual ~Texture();

	void render(const Renderer &renderer, const Shader &shader) const;

    int getWidth () const;
    int getHieght() const;
    int getBPP   () const;

    void   bind(const uint32_t slot = 0) const;
    void unBind() const;

	static std::array<float, 16> indexToVertex(uint16_t index);
};
