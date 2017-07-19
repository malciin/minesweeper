#include "Map.h"
#include <sstream>
// PUBLIC METHODS:
#include <iostream>
#include <iomanip>
Map::Map(int sizeY, int sizeX, int mines)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	mineTiles = mines;
	markedTiles = 0;
	hiddenTiles = sizeX * sizeY;
	lose = false;
	win = false;

	map.resize(sizeY + 2);
	for (int y = 0; y < sizeY + 2; y++)
		map[y].resize(sizeX + 2);

	// Setting horizontal border
	for (int y = 0; y < 2; y++)
		for (int x = 0; x < sizeX + 2; x++)
		{
			map[y*(sizeY + 1)][x].setValue(Tile::BORDER);
		}
			

	// Setting vertical border
	for (int y = 0; y < sizeY + 2; y++)
		for (int x = 0; x < 2; x++)
		{
			map[y][x*(sizeX + 1)].setValue(Tile::BORDER);
		}
			

	generateMap();
}

void Map::generateMap(int seed)
{
	srand(seed);
	markedTiles = 0;
	hiddenTiles = sizeX * sizeY;

	// Place bombs
	int bombsPlaced = 0;
	while (bombsPlaced != mineTiles)
	{
		int Y = std::rand() % (sizeY) + 1;
		int X = std::rand() % (sizeX) + 1;

		if (map[Y][X] == Tile::MINE)
			continue;
		
		map[Y][X].setValue(Tile::MINE);
		bombsPlaced++;
	}
	// Set value of all tiles and hide all tiles
	for(int y = 1; y<sizeY + 1; y++)
		for (int x = 1; x < sizeX + 1; x++)
		{
			map[y][x].setValue(bombsAround(y, x));
			map[y][x].setStatus(Tile::HIDDEN);
		}
			
}

void Map::reveal(int y, int x)
{
	if (map[y][x].getStatus() != Tile::REVEALED && map[y][x].getStatus() != Tile::MARKED)
	{
		if (map[y][x] == 0)
		{
			revealRecursive(y, x);
			checkWin();
			if (isWin())
				revealAllBombs();
			return;
		}

		hiddenTiles--;

		if ((map[y][x].getStatus() == Tile::MARKED || win) && map[y][x] == Tile::MINE)
			map[y][x].setMarkStatus(Tile::CORRECT_MARK);

		else if (map[y][x].getStatus() == Tile::MARKED && map[y][x] != Tile::MINE)
			map[y][x].setMarkStatus(Tile::WRONG_MARK);

		setStatus(y, x, Tile::REVEALED);
		if (map[y][x] == Tile::MINE && !win)
		{
			lose = true;
			revealAllBombs();
		}
			
		checkWin();
		if (isWin())
			revealAllBombs();
	}
}

void Map::revealRecursive(int y, int x)
{
	if (map[y][x] == 0 && map[y][x].getStatus()!=Tile::MARKED)
	{
		hiddenTiles--;
		setStatus(y, x, Tile::REVEALED);

		if (map[y - 1][x].getStatus() != Tile::REVEALED && map[y - 1][x] != -2)
			revealRecursive(y - 1, x);

		if (map[y + 1][x].getStatus() != Tile::REVEALED && map[y + 1][x] != -2)
			revealRecursive(y + 1, x);

		if (map[y][x - 1].getStatus() != Tile::REVEALED && map[y][x - 1] != -2)
			revealRecursive(y, x - 1);

		if (map[y][x + 1].getStatus() != Tile::REVEALED && map[y][x + 1] != -2)
			revealRecursive(y, x + 1);
	}
	else if (map[y][x] != Tile::BORDER && map[y][x].getStatus() != Tile::MARKED)
	{
		hiddenTiles--;
		setStatus(y, x, Tile::REVEALED);
	}
}

void Map::revealAllBombs()
{
	for (int y = 1; y <= sizeY; y++)
		for (int x = 1; x <= sizeX; x++)
		{
			if ((map[y][x].getStatus() == Tile::MARKED || win) && map[y][x] == Tile::MINE)
				map[y][x].setMarkStatus(Tile::CORRECT_MARK);

			else if (map[y][x].getStatus() == Tile::MARKED && map[y][x] != Tile::MINE)
			{
				map[y][x].setMarkStatus(Tile::WRONG_MARK);
				setStatus(y,x,Tile::REVEALED);
			}

			if (map[y][x] == Tile::MINE)
			setStatus(y, x, Tile::REVEALED);
		}
}

std::string Map::toString()
{
	std::string result;
	std::ostringstream stream;

	for (int y = 0; y < sizeY + 2; y++)
	{
		for (int x = 0; x < sizeX + 2; x++)
			stream << std::setw(3) << std::to_string(map[y][x]);
		stream << "\n";
	}

	result = stream.str();
	return result;
}

// PRIVATE METHODS:

int Map::bombsAround(int y, int x)
{
	int bombs = 0;

	// if actual position is a mine then return mine
	if (map[y][x] == Tile::MINE)
		return Tile::MINE;

	for (int Y = -1; Y < 2; Y++)
		for (int X = -1; X < 2; X++)
			if (map[y+Y][x+X] == Tile::MINE)
				bombs++;
	return bombs;
}