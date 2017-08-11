#pragma once
#include <SFML/Graphics.hpp>

class Notification : public sf::Drawable
{
	sf::Clock timer;
	sf::Text text;
	sf::RectangleShape background;
	int msNotificationDuration;
	sf::Vector2f rightBound;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (alive())
		{
			target.draw(background);
			target.draw(text);
		}
	}
public:
	void setFontSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}
	void setColor(const sf::Color & color, const sf::Color & background)
	{
		text.setColor(color);
		this->background.setFillColor(background);
	}
	void setFont(const sf::Font & font)
	{
		text.setFont(font);
	}
	void setPositions(const sf::Vector2f pos, const sf::Vector2f rightBound)
	{
		text.setPosition(pos);
		background.setPosition(pos);
		this->rightBound = rightBound;
	}
	void popupNotification(const sf::String & message, int miliSeconds)
	{
		msNotificationDuration = miliSeconds;
		timer.restart();
		text.setString(message);
		background.setSize(sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height));
	}

	void hide()
	{
		msNotificationDuration = 0;
	}

	bool alive() const
	{
		return timer.getElapsedTime().asMilliseconds() < msNotificationDuration;
	}
};