#include "Map.hpp"
#include <sstream>	// toString
#include <iomanip>	// toString
#include <set>		// revealIterative

// PUBLIC METHODS:
Map::Map(int sizeY, int sizeX, int mines)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	mineTiles = mines;

	map.resize(sizeY + 2);
	for (int y = 0; y < sizeY + 2; y++)
		map[y].resize(sizeX + 2);
			

	generateMap();
}

void Map::generateMap(int seed)
{
	srand(seed);
	markedTiles = 0;
	hiddenTiles = sizeX * sizeY;

	lose = false;
	win = false;

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

	for (int y = 1; y < sizeY + 1; y++)
		for (int x = 1; x < sizeX + 1; x++)
			map[y][x].setValue(0);


	// Place bombs
	int bombsPlaced = 0;
	if (mineTiles > hiddenTiles -1 )
	{
		mineTiles = hiddenTiles - 1;
	}
	std::vector<std::pair<int, int>> availableTiles;
	for (int i = 0; i < sizeY; i++)
		for (int j = 0; j < sizeX; j++)
			availableTiles.push_back(std::pair<int, int>(i + 1, j + 1));
	
	while (bombsPlaced != mineTiles)
	{
		int pos = std::rand() % availableTiles.size();
		
		int Y = availableTiles[pos].first;
		int X = availableTiles[pos].second;

		availableTiles[pos] = availableTiles[availableTiles.size() - 1];
		availableTiles.pop_back();
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
			map[y][x].setMarkStatus(Tile::NOTHING);
		}
}
void Map::mark(int y, int x)
{
	// INCREMENTED BECAUSE OF ADDITIONAL SIZE (TILE::BORDER) OF MAP
	y++;
	x++;
	if (y < 0 || x < 0 || y > sizeY || x > sizeX)
		throw std::out_of_range("Out of range");

	if (map[y][x].getStatus() == Tile::REVEALED)
		return;

	if (map[y][x].getStatus() != Tile::MARKED)
	{
		setStatus(y, x, Tile::MARKED);
		if (map[y][x] == Tile::MINE)
			map[y][x].setMarkStatus(Tile::CORRECT_MARK);
		else
			map[y][x].setMarkStatus(Tile::WRONG_MARK);
		markedTiles++;
	}
	else if (map[y][x].getStatus() == Tile::MARKED)
	{
		setStatus(y, x, Tile::HIDDEN);
		map[y][x].setMarkStatus(Tile::NOTHING);
		markedTiles--;
	}
}

void Map::setSize(int y, int x, int bombs)
{
	sizeY = y;
	sizeX = x;
	mineTiles = bombs;

	markedTiles = 0;
	hiddenTiles = sizeX * sizeY;
	lose = false;
	win = false;

	for (int i = 0; i < map.size(); i++)
		map[i].clear();
	map.clear();
	map.resize(sizeY + 2);
	for (int y = 0; y < sizeY + 2; y++)
		map[y].resize(sizeX + 2);
	
	generateMap();
	view->reload();
}

void Map::setMineNumber(int number)
{
	mineTiles = number;
}

void Map::reveal(int y, int x)
{
	// INCREMENTED BECAUSE OF ADDITIONAL SIZE (TILE::BORDER) OF MAP
	y++;
	x++;
	if (map[y][x].getStatus() != Tile::REVEALED && map[y][x].getStatus() != Tile::MARKED)
	{
		if (map[y][x] == 0)
		{
			revealIterative(y, x);
			checkWin();
			if (isWin())
				revealAllBombs();
			return;
		}

		hiddenTiles--;

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

void Map::revealIterative(int y, int x)
{
	typedef std::pair<int, int> vector;
	std::set<vector> points;
	points.insert(vector(y, x));
	while (!points.empty())
	{
		vector actual = *points.begin();
		int y = actual.first;
		int x = actual.second;
		points.erase(points.begin());
		
		if (map[y][x].getStatus() == Tile::MARKED)
			continue;
		setStatus(y, x, Tile::REVEALED);
		hiddenTiles--;

		if (map[y][x].getValue() == 0)
		{
			for (int yDelta = -1; yDelta < 2; yDelta += 2)
				if (map[y + yDelta][x].getStatus() != Tile::REVEALED && map[y + yDelta][x] != Tile::BORDER)
					points.insert(vector(y + yDelta, x));

			for (int xDelta = -1; xDelta < 2; xDelta += 2)
				if (map[y][x + xDelta].getStatus() != Tile::REVEALED && map[y][x + xDelta] != Tile::BORDER)
					points.insert(vector(y, x + xDelta));
		}
		
	}
}
void Map::revealDoubleClick(int y, int x)
{
	// INCREMENTED BECAUSE OF ADDITIONAL SIZE (TILE::BORDER) OF MAP
	y++;
	x++;
	if (map[y][x].getStatus() != Tile::REVEALED)
		return;
	int marked = 0;
	for (int yDelta = -1; yDelta < 2; yDelta++)
		for (int xDelta = -1; xDelta < 2; xDelta++)
			if (map[y+yDelta][x+xDelta].getStatus() == Tile::MARKED)
				marked++;
	if (map[y][x] != marked)
		return;
	for (int yDelta = -1; yDelta<2; yDelta++)
		for (int xDelta = -1; xDelta < 2; xDelta++)
		{
			if (map[y+yDelta][x+xDelta].getStatus() == Tile::HIDDEN)
				reveal(y + yDelta - 1, x + xDelta -1);
		}
}

void Map::revealAllBombs()
{
	for (int y = 1; y <= sizeY; y++)
		for (int x = 1; x <= sizeX; x++)
		{
			if (map[y][x].getMarkStatus() == Tile::WRONG_MARK)
				setStatus(y,x,Tile::REVEALED);

			if (win && map[y][x].getValue() == Tile::MINE)
				map[y][x].setMarkStatus(Tile::CORRECT_MARK);

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

