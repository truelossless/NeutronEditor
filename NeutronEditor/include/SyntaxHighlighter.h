#pragma once

#include "Line.h"

class SyntaxHighlighter
{
public:

	static void highlight(Line& line);
	static void highlightCharacter(Character& character, sf::Color& color);

private:
	static const char m_separators[];
	static const int m_separators_size = 16;

	static const std::string m_operators[];
	static const int m_operators_size = 8;

	static const std::string m_types[];
	static const int m_types_size = 15;

	static const char m_quotes[];
	static const int m_quotes_size = 3;

	static const std::string m_numbers[];
	static const int m_numbers_size = 10;
	static void checkArray(const std::string arr[], int size, Line& line, sf::Color& color, const std::string separators2[] = {}, int size2 = 0);
};