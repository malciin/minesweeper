#pragma once
#include <SFML/Graphics.hpp>

class Notification
{
	sf::Clock timer;
	sf::Text text;
	int msNotificationDuration;
	sf::Vector2f rightBound;
public:
	void setFontSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}
	void setColor(sf::Color & color)
	{
		text.setColor(color);
	}
	void setFont(sf::Font & font)
	{
		text.setFont(font);
	}
	void setPositions(const sf::Vector2f pos, const sf::Vector2f rightBound)
	{
		text.setPosition(pos);
		this->rightBound = rightBound;
	}
	void popupNotification(const sf::String & message, int miliSeconds)
	{
		timer.restart();
		text.setString(message);
	}

	bool alive()
	{
		return timer.getElapsedTime().asMilliseconds < msNotificationDuration;
	}

	const sf::Text & getDrawable() const
	{
		return text;
	}
};