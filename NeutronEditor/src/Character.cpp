#include "Character.h"


#include "Constants.h"

Character::Character(char character):
	m_text(character, Constants::FONT, Constants::FONT_SIZE)
{

}

Character::~Character()
{
}

sf::Text& Character::getText()
{
	return m_text;
}
