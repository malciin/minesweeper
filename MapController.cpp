#include "MapController.hpp"
#include "MapView.hpp"
void MapController::preceedMouse(sf::Vector2i clickPos, const sf::Event & event)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		if (!mouseDeltaActive)
			mouseDeltaActive = true;
		mouseDelta += event.mouseWheel.delta;
		if (mouseDelta < 0)
			mouseDelta = 0;
		if (mouseDelta > mapHandler->getMineUpperbound())
			mouseDelta = mapHandler->getMineUpperbound();
		std::cout << "Next generated map will have " << mouseDelta << "mines!\n";
	}
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

		clickPos.x /= viewHandler->getSquareSize() + viewHandler->getMargin();
		clickPos.y /= viewHandler->getSquareSize() + viewHandler->getMargin();
		if (clickPos.x >= mapHandler->getSizeX() ||
			clickPos.y >= mapHandler->getSizeY())
		{
			mouseCount = 0;
			return;
		}
		viewHandler->removeFocus();
		switch (mouseCount)
		{
		case LPM:
			mapHandler->reveal(clickPos.y, clickPos.x);
			break;
		case PPM:
			mapHandler->mark(clickPos.y, clickPos.x);
			break;
		case LPM_PPM:
			mapHandler->revealDoubleClick(clickPos.y, clickPos.x);
			break;
		}
		if (mouseCount != LPM_PPM)
			mouseCount = 0;
		else if (mouseCount == LPM_PPM)
			mouseCount = BLOCKED;
	}
}

void MapController::continiousMouse(sf::Vector2i pos)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		viewHandler->removeFocus();
		if (mapHandler->isWin() || mapHandler->isLose())
			return;

		if (pos.x < 0 || pos.y < 0)
			return;
		if (mouseCount == BLOCKED)
			return;
		pos.x /= viewHandler->getSquareSize() + viewHandler->getMargin();
		pos.y /= viewHandler->getSquareSize() + viewHandler->getMargin();
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
				viewHandler->addToFocusBuffor(sf::Vector2i(pos.x, pos.y));
		}
		else if (mouseCount == LPM_PPM)
		{
			for (int y = -1; y < 2; y++)
				for (int x = -1; x < 2; x++)
					if (mapHandler->getStatus(pos.y + y, pos.x + x) == Tile::HIDDEN)
						viewHandler->addToFocusBuffor(sf::Vector2i(pos.x + x, pos.y + y));
		}
		viewHandler->addFocus();
	}
	// UNBLOCK
	else if (mouseCount != 0)
		mouseCount = 0;
}


void MapController::preceedKeyboard(const sf::Event & event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			if (mouseDeltaActive)
			{
				mapHandler->setMineNumber(mouseDelta);
				std::cout << "Generated new!\n";
				return;
			}
			mapHandler->generateMap();
			viewHandler->reload();
		}
	}
}