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

	int downNotificationBarHeight = 10;
public:
	Minesweeper() : map(100, 100, 10)
	{
		mapView.handleMap(&map);
		mapView.setMarginBetweenSquares(0);
		mapView.setSquareSize(10);
		mapView.setMainColor(sf::Color::Blue);
		mapView.reload();

		downNotificationBarHeight = mapView.getFontSize() + 5;

		map.handleView(&mapView);

		mapController.handleMap(&map);
		mapController.handleView(&mapView);

		
		window = new sf::RenderWindow(sf::VideoMode(mapView.getSizeX(), mapView.getSizeY() + downNotificationBarHeight), "Minesweeper");
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
					int sizeX = (window->getSize().x) / (mapView.getMargin() + mapView.getSquareSize()) + int(mapView.getMargin() > 0);
					//int sizeX = (window->getSize().x + mapView.getMargin() * 2) / (mapView.getMargin() + mapView.getSquareSize());
					int sizeY = (window->getSize().y - downNotificationBarHeight) / (mapView.getMargin() + mapView.getSquareSize()) + int(mapView.getMargin() > 0);
					std::cout << "sizeX: " << sizeX << "==" << map.getSizeX() << "   " << "sizeY: " << sizeY << "== " << map.getSizeY() << "\n";
					if (sizeX != map.getSizeX() || sizeY != map.getSizeY())
					{
						int nextMines = mapController.getNextMineNumber();
						if (nextMines > map.getMineUpperbound())
						{
							nextMines = map.getMineUpperbound();
							mapController.setMineNumber(map.getMineUpperbound());
						}
						map.setSize(sizeY, sizeX, nextMines);

						mapView.popup(
							"Map " + std::to_string(map.getSizeX()) +
							"x" + std::to_string(map.getSizeY()) + " with " +
							std::to_string(map.getMineCount()) + " mines generated!", 3000);
					}
					window->setSize(sf::Vector2u(mapView.getSizeX(), mapView.getSizeY() + downNotificationBarHeight));

					// Proper scaling
					sf::View changedView(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
					window->setView(changedView);
				}
				if (event.type == sf::Event::Closed)
					window->close();
				mapController.preceedMouse(mouse.getPosition(*window), event);
				mapController.preceedKeyboard(event);
			}
			mapController.continiousMouse(mouse.getPosition(*window));

			mapView.updateBottomLine();
			window->clear();
			window->draw(mapView);
			window->display();
		}
	}
};