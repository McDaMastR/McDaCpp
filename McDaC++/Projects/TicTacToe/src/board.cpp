#include "include/board.hpp"

Board::Board(const std::string &file_path, const std::array<float, 16> &vertices, const std::array<std::uint32_t, 6> &indices)
	: Texture(file_path, vertices, indices) {}
