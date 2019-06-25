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

Line::~Line() {
}

Character& Line::getCharacter(int charNum) {
	return m_characters[charNum];
}

void Line::insert(char character, int charNum) {
	m_characters.insert(m_characters.begin() + charNum, character);
	m_line.insert(charNum, 1, character);

	resetIssues();
}

void Line::append(std::string text) {
	for (int i = 0; i < text.length(); i++) {
		m_characters.push_back(text[i]);
	}
	m_line += text;

	resetIssues();
}

void Line::erase(int charNum) {
	m_characters.erase(m_characters.begin() + charNum);
	m_line.erase(charNum, 1);

	resetIssues();
}

bool Line::errored()
{
	return m_errored;
}

bool Line::warned()
{
	return m_warned;
}

void Line::setErrored(bool errored) {
	m_errored = errored;
}

void Line::setWarned(bool warned) {
	m_warned = warned;
}

void Line::setIssue(std::string issue) {
	m_issue = issue;
}

std::string Line::getIssue() const {
	return m_issue;
}

std::string& Line::getText() {
	return m_line;
}

void Line::resetIssues() {
	m_warned = false;
	m_errored = false;
	m_issue = "";
}
