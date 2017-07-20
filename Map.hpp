#pragma once
#include "Tile.hpp"
#include <vector>
#include <string>
class IMapView
{
public:
	// Call when tile is changed
	virtual void notify(int y, int x) = 0;

	// Call only when map is just generated or map size is changed
	virtual void reload() = 0;
};

/*

	Main map class. This represent all map logic (generate minesweeper map, reveal tile, mark tile, double click reveal tile etc)

*/
class Map
{
	IMapView * view;

	std::vector<std::vector<Tile>> map;	// Map vector. This have Y(X) size 2 more than sizeY(sizeX) (to take care of border check)
							
	int sizeX, sizeY;

	int mineTiles;		// Number of mines
	int markedTiles;	// Number of marked tiles
	int hiddenTiles;	// Number of hidden tiles

	int bombsAround(int y, int x);						// Return number of bombs around tile. 
														// If that tile is actually a bomb this will return Tile::MINE
	void checkWin();									// Set the win boolean to true if map is resolved

	void revealIterative(int y, int x);					// Iterative reveal. Call only when user is clicked at the empty tile (no bombs around)
	void setStatus(int y, int x, Tile::Status status);	// Call everytime when you must change status of mine 
														// and View must be notified (this notify the view)
	void revealAllBombs();								// Reveal all bombs. Call only when user lose or win
	bool win;
	bool lose;
public:
	Map(int sizeY, int sizeX, int mines);	// Notice! std::vector<std::vector<Tile>> Y(X) size is 2 more than sizeY(sizeX).
											// This is to simplify border check (bordes will be equal Tile::BORDER)

	void handleView(IMapView * view);		// Handle the Interface Map View (needed for notify view)

	void generateMap();						// Generate random map
	void generateMap(int seed);				// Generate map from seed

	void reveal(int y, int x);				// Reveal the tile, and take care of the all special cases.
	void revealDoubleClick(int y, int x);	// Reveal the tiles when user click both mouse
	void mark(int y, int x);				// Mark or unmark the tile (set proper Mark Status)

	bool isLose();						// Return true if player lose
	bool isWin();						// Return true if player won

	std::string toString();				// Convert map to the string (usefull for std::cout debug)

	int getMineCount();					// Return mine number in the map
	int getSizeX();						// Return sizeX
	int getSizeY();						// Return sizeY

	void setSize(int y, int x, int mineNumber);	// Change map size and number of bombs, regenrate map and call view reload
	void setMineNumber(int number);

	int getValue(int y, int x);			// Get value of tile (bombs around, or -1 if tile is a mine)
	int getStatus(int y, int x);		// Get status of tile (if tile is hidden, marked or revealed)
	int getMarkStatus(int y, int x);	// Get status of mark (if mark is correct or wrong)
};

// INLINE METHODS
///..............

inline void Map::checkWin()
{
	win = (hiddenTiles == mineTiles) && !lose;
}

inline void Map::setStatus(int y, int x, Tile::Status status)
{
	if (y < 1 || x < 1 || x > sizeX + 1 || y > sizeY + 1)
		throw std::out_of_range("Out of range");
	map[y][x].setStatus(status);
	view->notify(y - 1, x - 1);
}

inline void Map::generateMap()
{
	generateMap(std::rand());
}

inline void Map::handleView(IMapView * view)
{
	this->view = view;
}

inline bool Map::isLose()
{
	return lose;
}
inline bool Map::isWin()
{
	return win;
}

inline int Map::getMineCount()
{
	return mineTiles;
}
inline int Map::getSizeX()
{
	return sizeX;
}
inline int Map::getSizeY()
{
	return sizeY;
}

inline int Map::getValue(int y, int x)
{
	if (y < -1 || x < -1 || y > sizeY || x > sizeX)
		throw std::out_of_range("Out of range");
	return map[y + 1][x + 1];
}
inline int Map::getStatus(int y, int x)
{
	if (y < -1 || x < -1 || y > sizeY || x > sizeX)
		throw std::out_of_range("Out of range");
	return map[y + 1][x + 1].getStatus();
}
inline int Map::getMarkStatus(int y, int x)
{
	if (y < -1 || x < -1 || y > sizeY || x > sizeX)
		throw std::out_of_range("Out of range");
	return map[y + 1][x + 1].getMarkStatus();
}