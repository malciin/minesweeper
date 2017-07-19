#pragma once
#include "Tile.hpp"
#include <vector>
#include <string>
class IMapView
{
public:
	virtual void notify(int y, int x) = 0;
	virtual void reload() = 0;
};


class Map
{
	IMapView * view;

	std::vector<std::vector<Tile>> map;
	int sizeX, sizeY;

	int mineTiles;
	int markedTiles;
	int hiddenTiles;

	int bombsAround(int y, int x);
	void checkWin()
	{
		win = (hiddenTiles == mineTiles) && !lose;
	}

	void revealIterative(int y, int x);
	void setStatus(int y, int x, Tile::Status status)
	{
		if (y < 1 || x < 1 || x > sizeX + 1 || y > sizeY + 1)
			throw std::out_of_range("Out of range");
		map[y][x].setStatus(status);
		view->notify(y - 1, x - 1);
	}


	bool win;
	bool lose;
public:
	Map(int sizeY, int sizeX, int mines);
	void generateMap()
	{
		generateMap(std::rand());
	}
	void generateMap(int seed);

	void reveal(int y, int x);
	void revealAllBombs();

	void handleView(IMapView * view)
	{
		this->view = view;
	}

	bool isLose()
	{
		return lose;
	}
	bool isWin()
	{
		return win;
	}

	std::string toString();

	int getMineCount()
	{
		return mineTiles;
	}
	int getSizeX()
	{
		return sizeX;
	}
	int getSizeY()
	{
		return sizeY;
	}

	int mark(int y, int x)
	{
		if (y < 0 || x < 0 || y > sizeY || x > sizeX)
			throw std::out_of_range("Out of range");
		if (map[y][x].getStatus() != Tile::MARKED)
		{
			setStatus(y, x, Tile::MARKED);
			markedTiles++;
		}
		else if (map[y][x].getStatus() == Tile::MARKED)
		{
			setStatus(y, x, Tile::HIDDEN);
			markedTiles--;
		}
		view->notify(y - 1, x - 1);
			
	}

	int getValue(int y, int x)
	{
		if (y < -1 || x < -1 || y > sizeY || x > sizeX)
			throw std::out_of_range("Out of range");
		return map[y+1][x+1];
	}
	int getStatus(int y, int x)
	{
		if (y < -1 || x < -1 || y > sizeY || x > sizeX)
			throw std::out_of_range("Out of range");
		return map[y + 1][x + 1].getStatus();
	}
	int getMarkStatus(int y, int x)
	{
		if (y < -1 || x < -1 || y > sizeY || x > sizeX)
			throw std::out_of_range("Out of range");
		return map[y + 1][x + 1].getMarkStatus();
	}

	bool isRevealed(int y, int x)
	{
		if (y < -1 || x < -1 || y > sizeY || x > sizeX)
			throw std::out_of_range("Out of range");
		return map[y + 1][x + 1].getStatus() == Tile::REVEALED;
	}
};