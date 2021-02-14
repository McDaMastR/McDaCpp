#pragma once

#include <vector>
#include <random>

enum class Difficulty
{
	easy = 0, medium, hard, sans
};


struct Position
{
	uint32_t i, j;
};


class Maze
{
public:
	Maze() noexcept = default;
	virtual ~Maze() noexcept = default;
	
	virtual void createMaze() = 0;
	virtual void solveMaze() = 0;
	
protected:
	// true == wall, false == space
	std::vector<std::vector<bool>> maze;
	std::random_device rand_gen;
	Position start, end;
};


class SquareMaze : public Maze
{
public:
	SquareMaze(const Difficulty difficulty) noexcept;
	SquareMaze(const uint32_t length) noexcept;
	~SquareMaze() noexcept = default;
	
	void createMaze();
	void solveMaze();
	
	uint16_t getLength() const noexcept;
};


class RectangleMaze : public Maze
{
public:
	RectangleMaze(const Difficulty difficulty) noexcept;
	RectangleMaze(const uint32_t height, const uint32_t width) noexcept;
	~RectangleMaze() noexcept = default;
	
	void createMaze();
	void solveMaze();

private:
	uint32_t height;
	uint32_t width;
};


class CircleMaze : public Maze
{
public:
	CircleMaze(const Difficulty difficulty) noexcept;
	CircleMaze(const uint32_t radius) noexcept;
	~CircleMaze() noexcept = default;
	
	void createMaze();
	void solveMaze();
	
private:
	uint32_t radius;
};
