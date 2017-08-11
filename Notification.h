#pragma once
#include <SFML/Graphics.hpp>

class Notification : public sf::Drawable
{
	sf::Clock timer;
	sf::Text text;
	int msNotificationDuration;
	sf::Vector2f rightBound;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (alive())
		{
			target.draw(text);
		}
	}
public:
	void setFontSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}
	void setColor(const sf::Color & color)
	{
		text.setColor(color);
	}
	void setFont(const sf::Font & font)
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

	bool alive() const
	{
		return timer.getElapsedTime().asMilliseconds() < msNotificationDuration;
	}
};