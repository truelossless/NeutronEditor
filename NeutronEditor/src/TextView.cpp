#include "TextView.h"

#include "Constants.h"
#include "SyntaxHighlighter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <nfd.h>

TextView::TextView(sf::RenderWindow& window) :
	m_window(window),
	m_view(window.getDefaultView()),
	m_absoluteXView(m_view),
	m_absoluteYView(m_view),
	m_cursor(),
	m_lineIndicator(sf::Vector2f(static_cast<float>(Constants::FONT_WIDTH * 3), static_cast<float>(window.getSize().y))) {

	m_width = window.getSize().x;
	m_height = window.getSize().y;
	m_lineIndicator.setFillColor(Constants::LINE_INDICATOR_BACKGROUND_COLOR);
}

TextView::~TextView()
{
}

void TextView::draw() {

	// where the text should be drawn
	int baseX = m_x + m_lineIndicator.getLocalBounds().width + Constants::FONT_WIDTH;

	// draw the highlighted line
	m_window.setView(m_absoluteXView);
	sf::RectangleShape currentLine(sf::Vector2f(m_width, Constants::FONT_SIZE));
	currentLine.setFillColor(Constants::CURRENT_LINE_COLOR);
	currentLine.setPosition(0, m_cursor.getLine()*Constants::FONT_SIZE);
	m_window.draw(currentLine);

	// draw all the text
	m_window.setView(m_view);

	// only draw visible lines
	int firstLine = m_window.mapPixelToCoords(sf::Vector2i(0, 0), m_view).y;
	firstLine = static_cast<int>(firstLine / Constants::FONT_SIZE);
	
	int lastLine = static_cast<int>(m_height / Constants::FONT_SIZE) + 1;
	lastLine += firstLine;
	if (lastLine > m_lines.size()) lastLine = m_lines.size();

	for (int i = firstLine; i < lastLine; i++) {
		for (int j = 0; j < m_lines[i].getText().length(); j++) {
			sf::Text character = m_lines[i].getCharacter(j).getText();
			character.setPosition(baseX + Constants::FONT_WIDTH*j, m_y + Constants::FONT_SIZE*i);
			m_window.draw(character);
		}
	}

	// draw the line indicator
	m_window.setView(Constants::absoluteView); // we can use here the absolute view
	m_lineIndicator.setSize(sf::Vector2f(m_x + (getLineIndicatorNumberCount() + 2)*Constants::FONT_WIDTH, m_y + m_height));
	m_window.draw(m_lineIndicator);

	// draw the line numbers
	m_window.setView(m_absoluteXView);
	for (int i = 0; i < m_lines.size(); i++) {
		sf::Text lineNumber(std::to_string(i + 1), Constants::FONT, Constants::FONT_SIZE);
		lineNumber.setPosition(m_x + Constants::FONT_WIDTH, m_y + Constants::FONT_SIZE*i);
		lineNumber.setFillColor(Constants::LINE_INDICATOR_TEXT_COLOR);
		m_window.draw(lineNumber);
	}

	// highlight the character under the cursor
	if (m_cursor.getPos() < getLine(m_cursor.getLine()).getText().length()) {
		SyntaxHighlighter::highlightCharacter(getLine(m_cursor.getLine()).getCharacter(m_cursor.getPos()), Constants::UNDER_CURSOR_COLOR);
	}

	// draw the cursor
	m_window.setView(m_view);
	m_cursor.getShape().setPosition(baseX + m_cursor.getPos() * Constants::FONT_WIDTH, Constants::FONT_SIZE * m_cursor.getLine());
	m_window.draw(m_cursor.getShape());
}

void TextView::updateView(int width, int height) {
	m_view.setSize(static_cast<float>(width), static_cast<float>(height));
	m_view.setCenter(static_cast<int>(width / 2), static_cast<int>(height / 2));

	m_absoluteXView = m_view;
	m_absoluteYView = m_view;

	m_width = width;
	m_height = height;
}

void TextView::moveView(int deltaX, int deltaY) {
	m_view.move(deltaX, deltaY);
	m_absoluteXView.move(0, deltaY);
	m_absoluteYView.move(deltaX, 0);
}

void TextView::scroll() {

	sf::Vector2i coords = m_window.mapCoordsToPixel(m_cursor.getShape().getPosition(), m_view);

	sf::Vector2f newCoords = m_view.getCenter();

	// scroll up
	if (coords.y < 0 && m_view.getCenter().y > m_height / 2) {
		newCoords.y = m_height / 2 + m_cursor.getLine() * Constants::FONT_SIZE;
	}

	// scroll down
	else if (coords.y + Constants::FONT_SIZE*3 > m_height + 5) {
		std::cout << m_height << " | " << coords.y + Constants::FONT_SIZE * 3 << std::endl;
		newCoords.y = - m_height / 2 + m_cursor.getShape().getGlobalBounds().top + Constants::FONT_SIZE*3;
	}

	// scroll left
	if (coords.x < m_lineIndicator.getLocalBounds().width) {
		newCoords.x = m_width / 2 + m_cursor.getShape().getGlobalBounds().left - m_lineIndicator.getLocalBounds().width;
		if (m_cursor.getPos() == 0) newCoords.x = m_width / 2;
	}
	// scroll right
	else if (coords.x > m_width) {
		newCoords.x = - m_width / 2 + m_cursor.getShape().getGlobalBounds().left + Constants::FONT_WIDTH;
	}

	m_view.setCenter(newCoords.x, newCoords.y);
	m_absoluteXView.setCenter(m_width / 2, newCoords.y);
	m_absoluteYView.setCenter(newCoords.x, m_height / 2);
	m_window.setView(m_view);
}

void TextView::clear() {
	m_lines = {
		Line("")
	};

	m_cursor.set(0, 0);
}

void TextView::save() {

	// if we don't know where to save this file, ask the user
	if (m_saveLocation.empty()) {

		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(NULL, NULL, &outPath);

		if (result == NFD_OKAY) {
	
			std::ofstream file(outPath, std::ios::trunc);
	
			for (int i = 0; i < m_lines.size(); i++) {
				file << m_lines[i].getText() << "\n";
			}
		
			m_saveLocation = *outPath;

			file.close();
			free(outPath);
		}
	}
	
	// now we should know where to save it, just check a last time, in case the prompt didn't succeed
	if (!m_saveLocation.empty()) {

		std::ofstream file(m_saveLocation, std::ios::trunc);

		for (int i = 0; i < m_lines.size(); i++) {
			file << m_lines[i].getText() << "\n";
		}
	}
}

void TextView::open() {

	nfdchar_t *inPath = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &inPath);

	if (result == NFD_OKAY) {

		// clear the textview
		clear();

		m_saveLocation = *inPath;

		std::ifstream file(m_saveLocation);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();
		paste(stream);

		free(inPath);

	}

}

void TextView::open(std::string path) {

	clear();
	std::ifstream file(path);
	std::stringstream iss;
	iss << file.rdbuf();
	file.close();
	m_saveLocation = path;
	paste(iss);
}

int TextView::getLineIndicatorNumberCount() {
	// right size for the line indicator
	int numberLength = 1;
	size_t lineNumber = m_lines.size();
	while (lineNumber /= 10) numberLength++;
	return numberLength;
}

void TextView::addLine(std::string line) {
	m_lines.push_back(Line(line));
}

void TextView::insertLine(int lineNum, Line& line) {
	m_lines.insert(m_lines.begin() + lineNum, line);
}

void TextView::insertLine(int lineNum) {
	Line line("");
	insertLine(lineNum, line);
}

void TextView::deleteLine(int lineNum) {
	m_lines.erase(m_lines.begin() + lineNum);
}

int TextView::enterInsertLine(int charNum, int lineNum) {

	std::string oldText = m_lines[lineNum].getText().substr(0, charNum);
	std::string newText = m_lines[lineNum].getText().substr(charNum);

	deleteLine(lineNum);

	Line oldLine(oldText);
	Line newLine(newText);


	SyntaxHighlighter::highlight(oldLine);
	SyntaxHighlighter::highlight(newLine);

	m_lines.insert(m_lines.begin() + lineNum, oldLine);
	m_lines.insert(m_lines.begin() + lineNum + 1, newLine);

	int pos = 0;
	while (pos < oldLine.getText().length() && oldLine.getText()[pos] == ' ') {
		m_lines[lineNum + 1].insert(' ', pos);
		pos++;
	}

	return pos;
}

int TextView::backspaceDeleteLine(int lineNum) {

	std::string previousText = m_lines[lineNum - 1].getText();
	std::string deletedLineText = m_lines[lineNum].getText();

	deleteLine(lineNum - 1);

	Line newLine(previousText + deletedLineText);
	SyntaxHighlighter::highlight(newLine);

	insertLine(lineNum - 1, newLine);
	deleteLine(lineNum);

	//return the new position of the cursor
	return static_cast<int>(previousText.length());
}

void TextView::paste(std::string& text) {
	std::stringstream iss(text);
	paste(iss);
}

void TextView::paste(std::stringstream& iss) {

	std::string tmp;
	int inserted = 0;

	if (std::getline(iss, tmp)) {

		m_lines[m_cursor.getLine()].append(tmp);
		SyntaxHighlighter::highlight(m_lines[m_cursor.getLine()]);
		inserted++;
	}

	while (std::getline(iss, tmp)) {

		// replace all tabs by spaces
		size_t start_pos = 0;
		while ((start_pos = tmp.find('\t', start_pos)) != std::string::npos) {
			tmp.replace(start_pos, 1, "    ");
			start_pos += 4;
			std::cout << "converted spaces";
		}

		Line line(tmp);
		insertLine(m_cursor.getLine() + inserted, line);
		SyntaxHighlighter::highlight(m_lines[m_cursor.getLine() + inserted]);
		inserted++;
	}

	m_cursor.setPos(0);
}

int TextView::getLineCount()
{
	return m_lines.size();
}

Cursor* TextView::getCursor()
{
	return &m_cursor;
}

void TextView::insert(char character, int charNum, int lineNum) {
	m_lines[lineNum].insert(character, charNum);
	SyntaxHighlighter::highlight(m_lines[lineNum]);
}

void TextView::erase(int charNum, int lineNum) {
	m_lines[lineNum].erase(charNum);
	SyntaxHighlighter::highlight(m_lines[lineNum]);
}

Line& TextView::getLine(int i) {
	return m_lines[i];
}

void TextView::setPosition(int x, int y) {
	m_x = x;
	m_y = y;
}

void TextView::setSize(int width, int height) {
	m_width = width;
	m_height = height;
}