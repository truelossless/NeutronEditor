#pragma once

#include <SFML/Graphics.hpp>

class Character
{
public:
	Character(char character);
	~Character();

	sf::Text& getText();

private:
	sf::Text m_text;
	sf::Color m_color;

};

