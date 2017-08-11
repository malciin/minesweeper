#pragma once
#include "Map.hpp"
#include <SFML/Graphics.hpp>
class MapView;
class MapController
{
	Map * mapHandler;		// Pointer to map (to perform actions of mark and reveal)
	MapView * viewHandler;	// Pointer to view (to perform focus at the tiles)

	int mouseCount;			// Mouse states. Look below
	enum mouseCount {
		LPM = 1,			// Left mouse pressed
		PPM,				// Right mouse pressed
		LPM_PPM,			// Left and right mouse pressed
		BLOCKED				// Special case after LPM_PPM - both must be released
	};

	bool mouseDeltaActive;
	int mouseDelta;
public:
	MapController() { mouseCount = 0; mouseDelta = 20; mouseDeltaActive = false; }
	void handleMap(Map * map)			// Handle the pointer to map
	{
		mapHandler = map;
		mouseDelta = mapHandler->getMineCount();
	}
	void handleView(MapView * view)		// Handle the pointer to view
	{
		viewHandler = view;
	}

	int getNextMineNumber()
	{
		return mouseDelta;
	}

	void setMineNumber(int mineNumber)
	{
		mouseDelta = mineNumber;
	}

	// Hotkeys
	void preceedKeyboard(const sf::Event & event);

	// Perform action when mouse button is released base on the mouseCount
	void preceedMouse(sf::Vector2i clickPos, const sf::Event & event);

	// Set the mouseCount and notify viewHandler to display the focus
	void continiousMouse(sf::Vector2i pos);
};