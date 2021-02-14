#include "maze.hpp"

SquareMaze::SquareMaze(const Difficulty mode) noexcept
{
	switch(mode)
	{
		case Difficulty::easy:
			maze.resize(11);
			break;
		
		case Difficulty::medium:
			maze.resize(26);
			break;
			
		case Difficulty::hard:
			maze.resize(51);
			break;
			
		default:
			maze.resize(81);
			break;
	}
	
	for(std::vector<bool> row : maze)
		row.resize(maze.size());
}

SquareMaze::SquareMaze(const uint32_t length) noexcept
{
	maze.resize(length);
	for(std::vector<bool> row : maze)
		row.resize(length);
}

void SquareMaze::createMaze()
{
	// Determine start and end of maze
	start.i = 0;
	end.i = static_cast<uint32_t>(maze.size() - 1);
	std::uniform_int_distribution<uint32_t> distribution(1, static_cast<uint32_t>(maze.size() - 2));
	start.j = distribution(rand_gen);
	end.j = distribution(rand_gen);
	
	// Turn outside of maze into walls
	for(uint32_t k = 0; k < maze.size(); k++)
	{
		if(!k)
		{
			for(uint32_t l = 0; l < maze.size(); l++)
			{
				if(l == start.j)
					continue;
				maze[k][l] = true;
				
			}
		}
		else if(k == maze.size() - 1)
		{
			for(uint32_t l = 0; l < maze.size(); l++)
			{
				if(l == end.j)
					continue;
				maze[k][l] = true;
			}
		}
		else
		{
			maze[k][0] = true;
			maze[k][maze.size() - 1] = true;
		}
	}
	
	// generate inside of maze
	for(uint32_t i = 1; i < maze.size() - 1; i++)
	{
		for(uint32_t j = 1; j < maze.size() - 1; j++)
		{
			if(i == 1 && j == start.j)
				maze[i][j] = false;
			else if(i == maze.size() - 2 && j == end.j)
				maze[i][j] = false;
			
			else if(i == 1)
			{
				if(j == 1)
				{
					if((maze[1][2] && maze[2][1]) || !(maze[1][2] && maze[2][1]))
						maze[1][1] = true;
					else
						maze[1][1] = false;
				}
				else if(j == maze.size() - 2)
				{
					if((maze[1][j - 1] && maze[2][j]) || !(maze[1][j - 1] && maze[2][j]))
						maze[1][j] = true;
					else
						maze[1][j] = false;
				}
				else
				{
					if(maze[1][j - 1])
				}
			}
		}
	}
	
	// Check maze has solution
}

uint16_t SquareMaze::getLength() const noexcept {return maze.size();}

RectangleMaze::RectangleMaze(const Difficulty mode) noexcept
{
	switch(mode)
	{
		case Difficulty::easy:
			height = 10;
			width = 15;
			break;
		
		case Difficulty::medium:
			height = 25;
			width = 40;
			break;
			
		case Difficulty::hard:
			height = 50;
			width = 75;
			break;
			
		default:
			height = 80;
			width = 110;
			break;
	}
	
	maze.resize(height);
	for(std::vector<bool> row : maze)
		row.resize(width);
}

RectangleMaze::RectangleMaze(const uint32_t height, const uint32_t width) noexcept
	: height(height), width(width)
{
	maze.resize(height);
	for(std::vector<bool> row : maze)
		row.resize(width);
}

CircleMaze::CircleMaze(const Difficulty mode) noexcept
{
	switch(mode)
	{
		case Difficulty::easy:
			radius = 7;
			break;
		
		case Difficulty::medium:
			radius = 16;
			break;
			
		case Difficulty::hard:
			radius = 35;
			break;
			
		default:
			radius = 68;
			break;
	}
	
	maze.resize(radius * 2);
	for(std::vector<bool> row : maze)
		row.resize(radius * 2);
}

CircleMaze::CircleMaze(const uint32_t radius) noexcept
	: radius(radius)
{
	maze.resize(radius * 2);
	for(std::vector<bool> row : maze)
		row.resize(radius * 2);
}
