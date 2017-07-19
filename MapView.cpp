#include "MapView.hpp"
MapView::MapView() : focusBuffor(10)
{
	squares.setPrimitiveType(sf::PrimitiveType::Quads);
	font.loadFromFile("ubuntu.ttf");
}
const int TILE_TEXTURES_COUNT = 12;
const int MINE_TEXTURE_NUMBER = 9;
const int GOOD_MARK_NUMBER = 10;
const int WRONG_MARK_NUMBER = 11;
void MapView::renderTextures()
{
	sf::RenderTexture render;
	render.create(squareSize * TILE_TEXTURES_COUNT, squareSize);
	render.clear(sf::Color::White);

	sf::Text text;
	text.setFont(font);
	text.setColor(sf::Color::Black);
	text.setCharacterSize(squareSize);


	// Value tiles from 0 - 8
	for (int i = 0; i < 9; i++)
	{
		if (i == 0)
			text.setString("");
		else
			text.setString(std::to_string(i));

		text.setPosition(sf::Vector2f(
			i*squareSize + (squareSize / 2 - text.getLocalBounds().width / 2),
			-(squareSize - text.getLocalBounds().height) / 2
		));
		render.draw(text);
	}
	// Mine Tile (9)
	sf::Texture mineTexture;
	mineTexture.loadFromFile("mine.png");
	sf::Sprite mine;
	mine.setTexture(mineTexture);
	mine.setScale(sf::Vector2f(squareSize / 300.0, squareSize / 300.0));
	mine.setPosition(squareSize * MINE_TEXTURE_NUMBER, 0);

	sf::RectangleShape background;
	background.setSize(sf::Vector2f(squareSize, squareSize));
	background.setFillColor(sf::Color::Red);
	background.setPosition(mine.getPosition());
	render.draw(background);
	render.draw(mine);

	// GoodMark (10)
	mine.setPosition(squareSize * GOOD_MARK_NUMBER, 0);
	background.setFillColor(sf::Color::Green);
	background.setPosition(mine.getPosition());
	render.draw(background);
	render.draw(mine);

	// WrongMark (11)
	mine.setPosition(squareSize * WRONG_MARK_NUMBER, 0);
	mine.setColor(sf::Color(0, 0, 0, 127));
	background.setFillColor(sf::Color::White);
	background.setPosition(mine.getPosition());
	render.draw(background);
	render.draw(mine);

	render.display();

	textures = render.getTexture();
}


void MapView::setColor(int y, int x, sf::Color color)
{
	sf::Vertex * square = &squares[(y*mapHandler->getSizeX() + x) * 4];
	for (int i = 0; i < 4; i++)
		square[i].color = color;
}

void MapView::setColor(int y, int x)
{
	int status = mapHandler->getStatus(y,x);
	int value = mapHandler->getValue(y, x);
	sf::Vertex * square = &squares[(y*mapHandler->getSizeX() + x) * 4];
	if (status == Tile::HIDDEN)
		setColor(y, x, mainColor);
	else if (status == Tile::MARKED)
		setColor(y, x, sf::Color::Red);
	else if (status == Tile::REVEALED)
		setColor(y, x, sf::Color::White);
}
void MapView::setTexture(int y, int x)
{
	int status = mapHandler->getStatus(y, x);
	int value = mapHandler->getValue(y, x);
	sf::Vertex * quad = &squares[(y*mapHandler->getSizeX() + x) * 4];
	if (status != Tile::REVEALED)
	{
		for (int i = 0; i < 4; i++)
			quad[i].texCoords = sf::Vector2f(0, 0);
		return;
	}
	

	if (mapHandler->getMarkStatus(y,x) == Tile::CORRECT_MARK)
	{
		quad[0].texCoords = sf::Vector2f(GOOD_MARK_NUMBER*squareSize, 0);
		quad[1].texCoords = sf::Vector2f(GOOD_MARK_NUMBER*squareSize + squareSize, 0);
		quad[2].texCoords = sf::Vector2f(GOOD_MARK_NUMBER*squareSize + squareSize, squareSize);
		quad[3].texCoords = sf::Vector2f(GOOD_MARK_NUMBER*squareSize, squareSize);
	}
	else if (mapHandler->getMarkStatus(y, x) == Tile::WRONG_MARK)
	{
		quad[0].texCoords = sf::Vector2f(WRONG_MARK_NUMBER*squareSize, 0);
		quad[1].texCoords = sf::Vector2f(WRONG_MARK_NUMBER*squareSize + squareSize, 0);
		quad[2].texCoords = sf::Vector2f(WRONG_MARK_NUMBER*squareSize + squareSize, squareSize);
		quad[3].texCoords = sf::Vector2f(WRONG_MARK_NUMBER*squareSize, squareSize);
	}
	else if (value == Tile::MINE)
	{
		quad[0].texCoords = sf::Vector2f(MINE_TEXTURE_NUMBER*squareSize, 0);
		quad[1].texCoords = sf::Vector2f(MINE_TEXTURE_NUMBER*squareSize + squareSize, 0);
		quad[2].texCoords = sf::Vector2f(MINE_TEXTURE_NUMBER*squareSize + squareSize, squareSize);
		quad[3].texCoords = sf::Vector2f(MINE_TEXTURE_NUMBER*squareSize, squareSize);
	}
	else if (value >= 0 && value <= 8)
	{
		quad[0].texCoords = sf::Vector2f(value*squareSize, 0);
		quad[1].texCoords = sf::Vector2f(value*squareSize + squareSize, 0);
		quad[2].texCoords = sf::Vector2f(value*squareSize + squareSize, squareSize);
		quad[3].texCoords = sf::Vector2f(value*squareSize, squareSize);
	}
}


void MapView::reload()
{
	squares.resize(4 * mapHandler->getSizeX()*mapHandler->getSizeY());

	sf::Vertex * quad;
	for (int y = 0; y < mapHandler->getSizeY(); y++)
		for (int x = 0; x < mapHandler->getSizeX(); x++)
		{
			quad = &squares[(y*mapHandler->getSizeX() + x) * 4];

			for (int i = 0; i < 4; i++)
			{
				quad[i].color = mainColor;
				quad[i].texCoords = sf::Vector2f(0, 0);
			}
				

			quad[0].position = sf::Vector2f(x*squareSize + x*margin, y*squareSize + y*margin);
			quad[1].position = sf::Vector2f((x + 1)*squareSize + x*margin, y*squareSize + y*margin);
			quad[2].position = sf::Vector2f((x + 1)*squareSize + x*margin, (y + 1)*squareSize + y*margin);
			quad[3].position = sf::Vector2f(x*squareSize + x*margin, (y + 1)*squareSize + y*margin);
		}
}
void MapView::notify(int y, int x)
{
	setColor(y, x);
	setTexture(y, x);
}


void MapView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = &textures;
	target.draw(squares, states);
}

void MapView::removeFocus()
{
	for (int i = 0; i < focusBuffor.getSize(); i++)
	{
		setColor(focusBuffor[i].y, focusBuffor[i].x);
	}
	focusBuffor.drop();
}

void MapView::addFocus()
{
	sf::Vertex * quad;
	for (int i = 0, n = focusBuffor.getSize(); i < n; i++)
	{
		sf::Vector2i pos = focusBuffor[i];
		setColor(pos.y, pos.x, sf::Color(mainColor.r, mainColor.g, mainColor.b, 128));
	}
}