#pragma once

#include <vector>

#include "Character.h"

class Line
{
public:
	Line(std::string text);
	Character& getCharacter(int charNum);
	void insert(char character, int charNum);
	void append(std::string text);
	void erase(int charNum);
	std::string& getText();
	~Line();

private:
	std::vector<Character> m_characters;
	std::string m_line;
};

