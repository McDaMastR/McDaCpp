#include "include/piece.hpp"

Piece::Piece(const std::string &file_path, const PieceType type, const uint16_t index, const std::array<uint32_t, 6> &indices)
	: Texture(file_path, index, indices), m_type(type), m_index(index) {}

void Piece::addPiece(const std::string &file_path, const PieceType type, const uint16_t index, const std::array<uint32_t, 6> &indices)
{
	pieces.emplace_back(Piece{file_path, type, index, indices});
}

void Piece::render() // TODO batch rendering
{

}

bool Piece::check()
{
	if (pieces.size() == 9)
		return true;
}
