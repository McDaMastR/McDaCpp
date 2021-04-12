#include "include/piece.hpp"

#include <iostream>

std::ostream &operator<<(std::ostream &os, const PieceType type)
{
	switch(type)
	{
	case PieceType::null:
		os << "NULL";
		break;

	case PieceType::X:
		os << "X";
		break;

	default:
		os << "O";
		break;
	}
	return os;
}

void swapPieceType(PieceType &type)
{
	type = static_cast<PieceType>(static_cast<std::int8_t>(type) * -1);
}

Piece::Piece(const PieceType type, const std::array<std::uint32_t, 6> &indices)
	: Texture((type == PieceType::X ? "res/textures/X.jpg" : "res/textures/O.jpg"), current_index, indices), 
	m_index(current_index), m_type(type) {}

std::uint8_t Piece::index() const
{
	return m_index;
}

PieceType Piece::type() const
{
	return m_type;
}

void Piece::addPiece(const PieceType type, const std::array<std::uint32_t, 6> &indices)
{
	pieces[current_index / 3][current_index % 3] = new Piece{type, indices};
	current_index++;
}

void Piece::renderAll(const Renderer &renderer) // TODO batch rendering
{
	for (std::uint8_t i = 0; i < 3; i++) {
		for (std::uint8_t j = 0; j < 3; j++) {
			if (pieces[i][j])
				pieces[i][j]->render(renderer);
			else
				goto PieceRenderAllEnd;
		}
	}
	PieceRenderAllEnd:
		return;
}

bool Piece::check()
{
	if (current_index == 9)
		return true;

	std::uint8_t no_X = 0, no_O = 0;
	for (std::uint8_t i = 0; i < 3; i++) {
		for (std::uint8_t j = 0; j < 3; j++) {
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
	for (std::uint8_t i = 0; i < 3; i++) {
		current = PieceType::null;
		for (std::uint8_t j = 0; j < 3; j++){
			if (pieces[i][j] == nullptr)
				goto CheckEnd2;
			if (current == PieceType::null)
				current = pieces[i][j]->type();
			else if (current != pieces[i][j]->type())
				break;
			else if (j == 2)
				return true;
		}
	}
	CheckEnd2:

	current = PieceType::null;
	for (std::uint8_t i = 0; i < 3; i++) {
		if (pieces[i][i] == nullptr)
			break;
		if (current == PieceType::null) 
			current = pieces[i][i]->type();
		else if (current != pieces[i][i]->type())
			break;
		else if (i == 2)
			return true;
	}

	current = PieceType::null;
	for (std::uint8_t i = 0, j = 2; i < 3; i++, j--) {
		if (pieces[i][i] == nullptr)
			break;
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
	for (std::uint8_t i = 0; i < 3; i++)
		for (std::uint8_t j = 0; j < 3; j++)
			delete pieces[i][j];
}
