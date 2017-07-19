#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>
#include "Buffor.h"
class MapView : public IMapView, public sf::Drawable
{
	Map * mapHandler;

	sf::VertexArray squares;
	sf::Color mainColor;
	sf::Font font;

	int squareSize;
	int margin;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Texture textures;

	void setColor(int y, int x, sf::Color color);
	void setColor(int y, int x);
	void setTexture(int y, int x);

	void updateTile(int y, int x);

	Buffor<sf::Vector2i> focusBuffor;
	void removeFocus();
	void addFocus();

	int mouseCount;
	enum mouseCount {
		LPM = 1,
		PPM,
		LPM_PPM,
		BLOCKED
	};
public:
	MapView();

	void renderTextures();

	void setMarginBetweenSquares(int size)
	{
		margin = size;
	}

	void setSquareSize(int size)
	{
		if (size > 300)
			throw std::logic_error("Square size cannot be more that 300px!");
		squareSize = size;
		renderTextures();
	}

	void handleMap(Map * map)
	{
		mapHandler = map;
	}

	int getSizeX()
	{
		return (squareSize + margin) * mapHandler->getSizeX() - margin;
	}
	int getSizeY()
	{
		return (squareSize + margin) * mapHandler->getSizeY() - margin;
	}

	void setMainColor(sf::Color color)
	{
		mainColor = color;
	}

	void preceedMouse(sf::Vector2i clickPos, sf::Event & event);
	void continiousMouse(sf::Vector2i pos);
	virtual void notify(int y, int x);
	virtual void reload();
};