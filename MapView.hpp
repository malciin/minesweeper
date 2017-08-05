#pragma once
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Buffor.hpp"
class MapView : public IMapView, public sf::Drawable
{
	Map * mapHandler;

	sf::VertexArray squares;
	sf::Color mainColor;
	
	int squareSize;
	int margin;

	sf::Font font;
	sf::Texture textures;
	void renderTextures();

	void setColor(int y, int x, sf::Color color);	// Set tile color to any color	
	void setColor(int y, int x);					// Set the tile color based at current map status	
													// (you can set all default tiles colors here)

	void setTexture(int y, int x);					// Set the tile texture
													// Here's mapped number textures to proper map value

	Buffor<sf::Vector2i> focusBuffor;				// Buffor needed to preceed focus on the tiles

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
	MapView();
	void handleMap(Map * map);						// set the mapHandler pointer

	// IMapView
	virtual void notify(int y, int x);				// Update tile (check IMapView description for more)
	virtual void reload();							// Reloading view (check IMapView description for more)
	///...................

	void setMarginBetweenSquares(int size);			// set the margin between the squares
	void setSquareSize(int size);					// set the square size

	void setMainColor(sf::Color color);

	// Getters
	int getSquareSize();
	int getMargin();
	int getSizeX();
	int getSizeY();
	///...................

	// Focus methods (called from MapController)
	void removeFocus();
	void addFocus();
	void addToFocusBuffor(const sf::Vector2i & vector);
};




// INLINE METHODS
///..............

inline void MapView::setMarginBetweenSquares(int size)
{
	margin = size;
}

inline void MapView::setSquareSize(int size)
{
	if (size > 300)
		throw std::logic_error("Square size cannot be more that 300px!");
	squareSize = size;
	renderTextures();
}

inline void MapView::handleMap(Map * map)
{
	mapHandler = map;
}

inline int MapView::getSizeX()
{
	return (squareSize + margin) * mapHandler->getSizeX() - margin;
}
inline int MapView::getSizeY()
{
	return (squareSize + margin) * mapHandler->getSizeY() - margin;
}
inline void MapView::setMainColor(sf::Color color)
{
	mainColor = color;
}
inline int MapView::getSquareSize()
{
	return squareSize;
}
inline int MapView::getMargin()
{
	return margin;
}
inline void MapView::addToFocusBuffor(const sf::Vector2i & vector)
{
	focusBuffor.push(vector);
}