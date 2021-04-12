#pragma once

#include "texture.hpp"

enum class PieceType : std::int8_t
{
	null = 0, 
	X = -1, O = 1
};

void swapPieceType(PieceType &type);

class Piece : public Texture
{
private:
	const std::uint8_t m_index;
	PieceType m_type;

	inline static std::array<std::array<Piece *, 3>, 3> pieces {{
		{nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr}, 
		{nullptr, nullptr, nullptr}
	}};
	inline static std::uint8_t current_index = 0;
	inline static PieceType winner = PieceType::null;

public:
	Piece(const PieceType type, const std::array<std::uint32_t, 6> &indices);
	~Piece() = default;

	std::uint8_t index() const;
	PieceType type() const;

	static void addPiece(const PieceType type, const std::array<std::uint32_t, 6> &indices);
	static void renderAll(const Renderer &renderer);
	static bool check();
	static void deletePieces();
};
