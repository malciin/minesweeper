#pragma once
#include "Map.hpp"
#include "MapView.hpp"
#include "MapController.hpp"
#include <iostream>
class Minesweeper
{
	sf::RenderWindow * window;
	Map map;
	MapView mapView;
	MapController mapController;

	sf::Mouse mouse;
public:
	Minesweeper() : map(20,30, 2)
	{
		mapView.handleMap(&map);
		mapView.setMarginBetweenSquares(2);
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
				// Change the map size when user resize the window
				
				if (event.type == event.Resized)
				{
					int sizeX = window->getSize().x / (mapView.getMargin() + mapView.getSquareSize()) + 1;
					int sizeY = window->getSize().y / (mapView.getMargin() + mapView.getSquareSize()) + 1;
					if (sizeX != map.getSizeX() || sizeY != map.getSizeY())
					{
						map.setSize(sizeY, sizeX, mapController.getNextMineNumber());

						window->setSize(sf::Vector2u(mapView.getSizeX(), mapView.getSizeY()));

						// Proper scaling
						sf::View changedView(sf::FloatRect(0, 0, mapView.getSizeX(), mapView.getSizeY()));
						window->setView(changedView);
					}
				}
				if (event.type == sf::Event::Closed)
					window->close();
				mapController.preceedMouse(mouse.getPosition(*window), event);
				mapController.preceedKeyboard(event);
			}
			
			mapController.continiousMouse(mouse.getPosition(*window));
			window->clear();
			window->draw(mapView);
			window->display();
		}
	}
};