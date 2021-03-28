#pragma once

#include "texture.hpp"

enum class PieceType : uint8_t
{
	null, X, O
};

void swapPiecetype(PieceType &type);

class Piece : public Texture
{
private:
	PieceType m_type;

	inline static std::array<std::array<Piece *, 3>, 3> pieces {{
		{nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr}, 
		{nullptr, nullptr, nullptr}
	}};
	inline static uint16_t index = 0;
	inline static PieceType winner = PieceType::null;

public:
	Piece(const PieceType type, const std::array<uint32_t, 6> &indices);
	~Piece() = default;

	PieceType type() const;

	static void addPiece(const PieceType type, const std::array<uint32_t, 6> &indices);
	static void renderAll(const Renderer &renderer, const Shader &shader);
	static bool check();
	static void deletePieces();
};
