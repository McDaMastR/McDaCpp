#include "include/piece.hpp"

void swapPiecetype(PieceType &type)
{
	if (type == PieceType::X)
		type = PieceType::O;
	else
		type = PieceType::X;
}

Piece::Piece(const PieceType type, const std::array<uint32_t, 6> &indices)
	: Texture((type == PieceType::X ? "res/textures/X.jpg" : "res/textures/O.jpg"), index, indices), m_type(type) {}

PieceType Piece::type() const
{
	return m_type;
}

void Piece::addPiece(const PieceType type, const std::array<uint32_t, 6> &indices)
{
	pieces[index % 3][index > 2 ?  (index - 3 > 2 ? index - 6 : index - 3)  : index] = new Piece{type, indices};
	index++;
}

void Piece::renderAll(const Renderer &renderer, const Shader &shader) // TODO batch rendering
{
	for (uint16_t i = 0; i < 3; i++) {
		for (uint16_t j = 0; j < 3; j++) {
			if (pieces[i][j])
				pieces[i][j]->render(renderer, shader);
			else
				goto RenderEnd;
		}
	}
	RenderEnd:
		return;
}

bool Piece::check()
{
	if (index == 8)
		return true;

	uint16_t no_X = 0, no_O = 0;
	for (uint16_t i = 0; i < 3; i++) {
		for (uint16_t j = 0; j < 3; j++) {
			if (!pieces[i][j])
				goto CheckEnd;
			if (pieces[i][j]->type() == PieceType::X)
				no_X++;
			else
				no_O++;
		}
	}
	CheckEnd:
		if ((no_X < 3) || (no_O < 3))
			return false;

	PieceType current;
	for (uint16_t i = 0; i < 3; i++) {
		current = PieceType::null;
		for (uint16_t j = 0; j < 3; j++){
			if (current == PieceType::null)
				current = pieces[i][j]->type();
			else if (current != pieces[i][j]->type())
				break;
			else if (j == 2)
				return true;
		}
	}

	current = PieceType::null;
	for (uint16_t i = 0; i < 3; i++) {
		if (current == PieceType::null) 
			current = pieces[i][i]->type();
		else if (current != pieces[i][i]->type())
			break;
		else if (i == 2)
			return true;
	}

	current = PieceType::null;
	for (uint16_t i = 0, j = 2; i < 3; i++, j--) {
		if (current == PieceType::null)
			current = pieces[i][j]->type();
		else if (current != pieces[i][j]->type() )
			break;
		else if (i == 2)
			return true;
	}
	return false;
}

void Piece::deletePieces()
{
	for (uint16_t i = 0; i < 3; i++)
		for (uint16_t j = 0; j < 3; j++)
			delete pieces[i][j];
}
