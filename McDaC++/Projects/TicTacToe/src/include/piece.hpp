#pragma once

#include "texture.hpp"

#include <vector>

enum class PieceType
{
	X, O
};

class Piece : public Texture
{
private:
	const PieceType m_type;
	const uint16_t m_index;
	inline static std::vector<Piece> pieces;

public:
	Piece(const std::string &file_path, const PieceType type, const uint16_t index, const std::array<uint32_t, 6> &indices);
	~Piece() = default;

	static void addPiece(const std::string &file_path, const PieceType type, const uint16_t index, const std::array<uint32_t, 6> &indices);
	static void render();
	static bool check();
};
