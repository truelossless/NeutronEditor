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
	bool errored();
	void setErrored(bool errored);
	std::string& getText();
	~Line();

private:
	std::vector<Character> m_characters;
	std::string m_line;
	bool m_errored = false;
};

