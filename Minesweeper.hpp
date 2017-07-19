#pragma once
#include "Map.hpp"
#include "MapView.hpp"

class Minesweeper
{
	sf::RenderWindow * window;
	Map map;
	MapView mapView;

	sf::Mouse mouse;
public:
	Minesweeper() : map(200,200, 10)
	{
		mapView.handleMap(&map);
		mapView.setMarginBetweenSquares(1);
		mapView.setSquareSize(3);
		mapView.setMainColor(sf::Color::Blue);
		mapView.reload();

		map.handleView(&mapView);

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
				mapView.preceedMouse(mouse.getPosition(*window), event);
			}
			
			mapView.continiousMouse(mouse.getPosition(*window));
			window->clear();
			window->draw(mapView);
			window->display();
		}
	}
};