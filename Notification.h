#pragma once
#include <SFML/Graphics.hpp>
#include "Buffor.hpp"
#include <sstream>
#include <iomanip>
class Notification : public sf::Drawable
{
	sf::Clock timer;
	std::vector<sf::Text> row;

	sf::Text textPattern;

	sf::Text bottomLineTime;
	sf::Text bottomLineRemain;

	sf::RectangleShape background;
	int msNotificationDuration;
	sf::Vector2f bound;

	float time;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (alive())
		{
			target.draw(background);
			for(auto line : row)
			target.draw(line);
		}
		target.draw(bottomLineTime);
		target.draw(bottomLineRemain);
	}
public:
	void setFontSize(unsigned int size)
	{
		textPattern.setCharacterSize(size);
		bottomLineRemain.setCharacterSize(size);
		bottomLineTime.setCharacterSize(size);
	}
	void setColor(const sf::Color & color, const sf::Color & background)
	{
		textPattern.setColor(color);
		bottomLineRemain.setColor(color);
		bottomLineTime.setColor(color);
		this->background.setFillColor(background);
	}
	void setFont(const sf::Font & font)
	{
		textPattern.setFont(font);
		bottomLineRemain.setFont(font);
		bottomLineTime.setFont(font);
	}
	void setBoundary(const sf::Vector2f bound)
	{
		this->bound = bound;
		bottomLineTime.setPosition(0, bound.y);
	}
	void popupNotification(const sf::String & message, int miliSeconds)
	{
		msNotificationDuration = miliSeconds;
		
		// Remove all lines
		row.clear();

		/// Split string 
		//  by white spaces to vector table
		std::stringstream ss(message);
		std::vector<std::string> words;
		std::string buf;
		while (ss >> buf)
			words.push_back(buf);

		// Add at every word whitespace
		for (int i = 0; i < words.size() - 1; i++)
			words[i] += " ";
		/// End of splitting

		// Set of temporary text same apperance
		sf::Text temporary = textPattern;

		int widestStringID = 0;
		for (int wordIndex = 0; wordIndex < words.size();)
		{
			temporary.setString("");	// set empty temporary string
			std::string line;			// actuall line
			std::string matchingLine;	// line that do not exceed boundary
			while (temporary.getLocalBounds().width < bound.x)	// when we do not exceed a line
			{
				line += words[wordIndex];	// add a next word to line
				temporary.setString(line);	// set string in teporary text
				if (temporary.getLocalBounds().width < bound.x)
				{	// if we do not exceed a line with new string
					matchingLine = line;	// set new matching string
					wordIndex++;			// next counter up!
				}
				if (wordIndex == words.size())
					break;
			}
			// after having maximum length string that do not exceed a line
			// we remove white space at the end of that ending string
			if (matchingLine[matchingLine.size() - 1] == ' ')
				matchingLine.resize(matchingLine.size() - 1);

			
			textPattern.setString(matchingLine);	// and we set that string
			// compute center position
			textPattern.setPosition(sf::Vector2f((bound.x - textPattern.getLocalBounds().width) / 2, row.size()*textPattern.getCharacterSize()));
			row.push_back(textPattern);	// adding that string to row vector

			if (textPattern.getLocalBounds().width > row[widestStringID].getLocalBounds().width)
				widestStringID = row.size() - 1;
			
			background.setPosition(sf::Vector2f(row[widestStringID].getPosition().x - 2, 0));
			background.setSize(sf::Vector2f(
				row[widestStringID].getLocalBounds().width + 4,
				textPattern.getCharacterSize() * row.size() + 5));
		}

		timer.restart();
	}

	void hide()
	{
		msNotificationDuration = 0;
		row.clear();
	}

	void update(float time, int remainMines)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(4) << time;
		std::string timeStr = stream.str();

		bottomLineTime.setString("Sec: " + timeStr);
		bottomLineRemain.setString("Mines: " + std::to_string(remainMines));
		bottomLineRemain.setPosition(bound.x - bottomLineRemain.getLocalBounds().width, bound.y);
	}

	bool alive() const
	{
		return timer.getElapsedTime().asMilliseconds() < msNotificationDuration;
	}
};