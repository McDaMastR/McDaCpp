#pragma once

#include "texture.hpp"

class Board : public Texture
{
public:
	Board(const std::string &file_path, const std::array<float, 16> &vertices, const std::array<uint32_t, 6> &indices);
	~Board() = default;
};
