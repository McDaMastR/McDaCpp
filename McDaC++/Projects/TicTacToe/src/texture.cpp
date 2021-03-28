#include "include/texture.hpp"

#include "vendor/stb_image/stb_image.h"

#include <GL/glew.h>
#include <iostream>

template <class T, std::size_t S>
static std::ostream &operator<<(std::ostream &os, const std::array<T, S> array)
{
	for (unsigned short i = 0, j = 0; i < S; i++, j++)
	{
		if (i + 1 != S)
			os << array[i] << ", ";
		else if (j == 3) {
			j = 0;
			os << array[i] << ", \n";
		} else
			os << array[i] << "\n\n";
	}
	return os;
}

Texture::Texture(const std::string &file_path, const std::array<float, 16> &vertices, const std::array<uint32_t, 6> &indices)
    : m_vertices(vertices), m_indices(indices), m_vbo(m_vertices.data(), sizeof(float) * 16), 
	m_ibo(m_indices.data(), 6), m_filePath(file_path), 
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

Texture::Texture(const std::string &file_path, const uint16_t index, const std::array<uint32_t, 6> &indices)
	: m_vertices(indexToVertex(index)), m_indices(indices), m_vbo(m_vertices.data(), sizeof(float) * 16), 
	m_ibo(m_indices.data(), 6), m_filePath(file_path),
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

	std::cout << m_vertices;
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &m_ID));
}

void Texture::render(const Renderer &renderer, const Shader &shader) const
{
	bind();
	m_vao.bind();
	m_vbo.bind();
	renderer.draw(m_vao, m_ibo, shader);
}

int Texture::getWidth() const
{
    return m_width;
}
int Texture::getHieght() const
{
    return m_height;
}
int Texture::getBPP() const
{
    return m_BPP;
}

void Texture::bind(const uint32_t slot) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::unBind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

std::array<float, 16> Texture::indexToVertex(uint16_t index)
{
	auto getPos = [=](const bool y, const bool null) mutable -> float {
		if (null) {
			if (y) 
				return (index % 3); // TODO fix this
			while (index > 2)
				index -= 3;
			return index * (-2.0f / 3.0f);
		}
		if (y) 
			return (index % 3) * (-2.0f / 3.0f) - 2.0f / 3.0f;
		while (index > 2)
			index -= 3;
		return index * (-2.0f / 3.0f);	
	};

	return 
	{
		getPos(false, true),  getPos(true, true),  0.0f, 0.0f,
		getPos(false, false), getPos(true, true),  1.0f, 0.0f,
		getPos(false, false), getPos(true, false), 1.0f, 1.0f,
		getPos(false, true),  getPos(true, false), 0.0f, 1.0f
	};
}
