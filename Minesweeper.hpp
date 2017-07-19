#pragma once
#include "Map.hpp"
#include "MapView.hpp"
#include "MapController.hpp"

class Minesweeper
{
	sf::RenderWindow * window;
	Map map;
	MapView mapView;
	MapController mapController;

	sf::Mouse mouse;
public:
	Minesweeper() : map(10,10, 10)
	{
		mapView.handleMap(&map);
		mapView.setMarginBetweenSquares(1);
		mapView.setSquareSize(25);
		mapView.setMainColor(sf::Color::Blue);
		mapView.reload();

		map.handleView(&mapView);

		mapController.handleMap(&map);
		mapController.handleView(&mapView);

		window = new sf::RenderWindow(sf::VideoMode(mapView.getSizeX(), mapView.getSizeY()), "Minesweeper");
		window->setFramerateLimit(60);
	}
	void mainLoop()
	{
		while (window->isOpen())
		{
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
				mapController.preceedMouse(mouse.getPosition(*window), event);
			}
			
			mapController.continiousMouse(mouse.getPosition(*window));
			window->clear();
			window->draw(mapView);
			window->display();
		}
	}
};