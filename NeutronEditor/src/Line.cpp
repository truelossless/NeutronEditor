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
	m_characters.insert(m_characters.begin() + charNum, character);
	m_line.insert(charNum, 1, character);

	m_errored = false;
}

void Line::append(std::string text) {
	for (int i = 0; i < text.length(); i++) {
		m_characters.push_back(text[i]);
	}
	m_line += text;

	m_errored = false;
}

void Line::erase(int charNum) {
	m_characters.erase(m_characters.begin() + charNum);
	m_line.erase(charNum, 1);

	m_errored = false;
}

bool Line::errored()
{
	return m_errored;
}

void Line::setErrored(bool errored) {
	m_errored = errored;
}

std::string& Line::getText()
{
	return m_line;
}

Line::~Line()
{
}
