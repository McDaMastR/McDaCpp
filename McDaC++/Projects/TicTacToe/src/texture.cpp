#include "include/texture.hpp"

#include "vendor/stb_image/stb_image.h"

#include <GL/glew.h>
#include <iostream>

// template <class T, std::size_t S>
// static std::ostream &operator<<(std::ostream &os, const std::array<T, S> array)
// {
// 	for (unsigned short i = 0, j = 0; i < S; i++, j++)
// 	{
// 		if (i + 1 != S)
// 			os << array[i] << ", ";
// 		else if (j == 3) {
// 			j = 0;
// 			os << array[i] << ", \n";
// 		} else
// 			os << array[i] << "\n\n";
// 	}
// 	return os;
// }

Texture::Texture(const std::string &file_path, const std::array<float, 16> &vertices, const std::array<std::uint32_t, 6> &indices)
    : m_vertices(vertices), m_indices(indices), m_ibo(m_indices.data(), 6),
	m_vbo(m_vertices.data(), sizeof(float) * 16), m_filePath(file_path), 
	m_localBuffer(stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4))
{
    GLCALL(glGenTextures(1, &m_ID));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_ID));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer)
        stbi_image_free(m_localBuffer);

	m_vao.bind();
	m_vbo.bind();
	m_ibo.bind();

	m_layout.push<float>(2);
	m_layout.push<float>(2);
	m_vao.addBuffer(m_vbo, m_layout);
}

Texture::Texture(const std::string &file_path, const std::uint8_t index, const std::array<std::uint32_t, 6> &indices)
	: m_vertices(indexToVertex(index)), m_indices(indices), m_ibo(m_indices.data(), 6), 
	m_vbo(m_vertices.data(), sizeof(float) * 16), m_filePath(file_path), 
	m_localBuffer(stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4))
{
    GLCALL(glGenTextures(1, &m_ID));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_ID));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer)
        stbi_image_free(m_localBuffer);
	
	m_vao.bind();
	m_vbo.bind();
	m_layout.push<float>(2);
	m_layout.push<float>(2);
	m_vao.addBuffer(m_vbo, m_layout);
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &m_ID));
}

void Texture::render(const Renderer &renderer) const
{
	bind();
	m_vao.bind();
	renderer.draw(m_ibo.getCount());
}

std::int32_t Texture::getWidth() const
{
    return m_width;
}

std::int32_t Texture::getHieght() const
{
    return m_height;
}

std::int32_t Texture::getBPP() const
{
    return m_BPP;
}

void Texture::bind(const std::uint32_t slot) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::unBind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

std::array<float, 16> Texture::indexToVertex(std::uint16_t index)
{
	auto getPos = [=](const bool left) -> float {
		return (index % 3) * 2.0f / 3.0f + left * 2.0f / 3.0f - 1;
	};

	return 
	{
		getPos(true),  getPos(true),  0.0f, 0.0f,
		getPos(false), getPos(true),  1.0f, 0.0f,
		getPos(false), getPos(false), 1.0f, 1.0f,
		getPos(true),  getPos(false), 0.0f, 1.0f
	};
}
