#include "MapView.hpp"

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

void MapView::preceedMouse(sf::Vector2i clickPos, sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (mapHandler->isWin() || mapHandler->isLose())
		{
			mouseCount = 0;
			return;
		}

		if (clickPos.x < 0 || clickPos.y < 0)
		{
			mouseCount = 0;
			return;
		}

		clickPos.x /= squareSize + margin;
		clickPos.y /= squareSize + margin;
		if (clickPos.x >= mapHandler->getSizeX() ||
			clickPos.y >= mapHandler->getSizeY())
		{
			mouseCount = 0;
			return;
		}
		removeFocus();
		switch (mouseCount)
		{
		case LPM:
			mapHandler->reveal(clickPos.y + 1, clickPos.x + 1);
			break;
		case PPM:
			if (mapHandler->getStatus(clickPos.y, clickPos.x) == Tile::HIDDEN)
				mapHandler->mark(clickPos.y + 1, clickPos.x + 1);
			break;
		case LPM_PPM:
			if (mapHandler->getStatus(clickPos.y, clickPos.x) != Tile::REVEALED)
				break;
			int marked = 0;
			for (int y = -1; y < 2; y++)
				for (int x = -1; x < 2; x++)
					if (mapHandler->getStatus(clickPos.y + y, clickPos.x + x) == Tile::MARKED)
						marked++;
			if (mapHandler->getValue(clickPos.y, clickPos.x) != marked)
				break;
			for (int y = -1; y<2; y++)
				for (int x = -1; x < 2; x++)
				{
					if (mapHandler->getStatus(clickPos.y + y, clickPos.x + x) == Tile::HIDDEN)
						mapHandler->reveal(clickPos.y + 1 + y, clickPos.x + 1 + x);
				}
			break;
		}
		if (mouseCount != LPM_PPM)
			mouseCount = 0;
		else if (mouseCount == LPM_PPM)
			mouseCount = BLOCKED;
	}
}

void MapView::continiousMouse(sf::Vector2i pos)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		removeFocus();
		if (mapHandler->isWin() || mapHandler->isLose())
			return;

		if (pos.x < 0 || pos.y < 0)
			return;
		if (mouseCount == BLOCKED)
			return;
		pos.x /= squareSize + margin;
		pos.y /= squareSize + margin;
		if (pos.x >= mapHandler->getSizeX() ||
			pos.y >= mapHandler->getSizeY())
			return;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseCount != LPM)
		{
			if (mouseCount == 0)
				mouseCount = LPM;
			else
				mouseCount = LPM_PPM;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mouseCount != PPM)
		{
			if (mouseCount == 0 || mouseCount == PPM)
				mouseCount = PPM;
			else
				mouseCount = LPM_PPM;
		}

		if (mouseCount == LPM)
		{
			if (mapHandler->getStatus(pos.y, pos.x) == Tile::HIDDEN)
				focusBuffor.push(sf::Vector2i(pos.x, pos.y));
		}
		else if (mouseCount == LPM_PPM)
		{
			for (int y = -1; y < 2; y++)
				for (int x = -1; x < 2; x++)
					if (mapHandler->getStatus(pos.y + y, pos.x + x) == Tile::HIDDEN)
						focusBuffor.push(sf::Vector2i(pos.x + x, pos.y + y));
		}
		addFocus();
	}
	// UNBLOCK
	else if (mouseCount!=0)
		mouseCount = 0;
}