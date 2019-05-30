#include "Line.h"

#include "Constants.h"
#include "Character.h"

#include <iostream>

Line::Line(std::string text) {
	m_line = text;
	for (int i = 0; i < text.length(); i++) {
		Character character(m_line[i]);
		m_characters.push_back(character);
	}
}

Character& Line::getCharacter(int charNum) {
	return m_characters[charNum];
}

void Line::insert(char character, int charNum) {
	std::cout << charNum;
	m_characters.insert(m_characters.begin() + charNum, character);
	m_line.insert(charNum, 1, character);
}

void Line::append(std::string text) {
	for (int i = 0; i < text.length(); i++) {
		m_characters.push_back(text[i]);
	}
	m_line += text;
}

void Line::erase(int charNum) {
	m_characters.erase(m_characters.begin() + charNum);
	m_line.erase(charNum, 1);
}

std::string& Line::getText()
{
	return m_line;
}

Line::~Line()
{
}
