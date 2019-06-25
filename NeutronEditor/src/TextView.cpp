#include "TextView.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <nfd.h>

#include "ActionBar.h"
#include "Constants.h"
#include "SyntaxHighlighter.h"
#include "Project.h"

std::vector<TextView*> TextView::m_textViews;
int TextView::m_currentTextViewIndex = 0;

TextView::TextView(sf::RenderWindow& window, ActionBar& actionBar, float rx, float ry, float rwidth, float rheight) :
	m_window(window),
	m_actionBar(actionBar),
	m_cursor(),
	m_linter(*this),

	m_rx(rx),
	m_ry(ry),
	m_rwidth(rwidth),
	m_rheight(rheight),

	m_lineIndicator(sf::Vector2f(Constants::FONT_WIDTH * 3, m_height)) {

	m_lineIndicator.setFillColor(Constants::LINE_INDICATOR_BACKGROUND_COLOR);
	updateView();

	// add a first line to the textarea
	addLine("");
}

TextView::~TextView()
{
}

void TextView::draw() {

	// draw a thin border to differenciate editors

	// draw the highlighted line
	if (m_active) {
		m_window.setView(m_absoluteXView);
		sf::RectangleShape currentLine(sf::Vector2f(m_width, Constants::FONT_SIZE));
		currentLine.setFillColor(Constants::CURRENT_LINE_COLOR);
		currentLine.setPosition(0, m_cursor.getLine()*Constants::FONT_SIZE);
		m_window.draw(currentLine);
	}

	// start of the text region
	int baseX = m_lineIndicator.getLocalBounds().width + Constants::FONT_WIDTH;

	// draw all the text
	m_window.setView(m_view);

	// draw all the characters
	for (int i = 0; i < m_lines.size(); i++) {

		// if the line is errored, draw a red background
		// TODO: register this color in the theme
		if (m_lines[i].errored() || m_lines[i].warned()) {
			sf::RectangleShape issueBg(sf::Vector2f(m_width, Constants::FONT_SIZE));

			if (m_lines[i].warned()) {
				issueBg.setFillColor(sf::Color(226, 169, 0, 50));
			}
			else if (m_lines[i].errored()) {
				issueBg.setFillColor(sf::Color(255, 0, 0, 50));
			}

			issueBg.setPosition(0, i*Constants::FONT_SIZE);
			m_window.draw(issueBg);
		}

		for (int j = 0; j < m_lines[i].getText().length(); j++) {
			sf::Text character = m_lines[i].getCharacter(j).getText();
			character.setPosition(baseX + Constants::FONT_WIDTH*j, Constants::FONT_SIZE*i);
			m_window.draw(character);
		}
	}

	// if the line under the cursor has an issue, display it in the actionbar
	if (m_lines[m_cursor.getLine()].errored() || m_lines[m_cursor.getLine()].warned()) {
		m_actionBar.setInfo(m_lines[m_cursor.getLine()].getIssue());
	}
	else {
		m_actionBar.setInfo("");
	}

	// draw the line indicator
	m_window.setView(m_absoluteView); // we can use here the absolute view
	m_lineIndicator.setSize(sf::Vector2f((getLineIndicatorNumberCount() + 2)*Constants::FONT_WIDTH, m_height));
	m_window.draw(m_lineIndicator);

	// draw the line numbers
	m_window.setView(m_absoluteXView);
	for (int i = 0; i < m_lines.size(); i++) {
		sf::Text lineNumber(std::to_string(i + 1), Constants::FONT, Constants::FONT_SIZE);
		lineNumber.setPosition(Constants::FONT_WIDTH, Constants::FONT_SIZE*i);
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

void TextView::updateView() {

	int width = m_window.getSize().x;
	int height = m_window.getSize().y;

	m_x = m_rx * width;
	m_y = m_ry * height;

	m_width = width * m_rwidth;
	m_height = height * m_rheight;

	std::cout << "width: " << m_width;
	std::cout << "height: " << m_height;

	m_view.setViewport(sf::FloatRect(m_rx, m_ry, m_rwidth, m_rheight));
	m_view.setSize(m_width, m_height);
	m_view.setCenter(m_width / 2, m_height / 2);

	m_absoluteView = m_view;
	m_absoluteXView = m_view;
	m_absoluteYView = m_view;

	scroll();
}

void TextView::moveView(int deltaX, int deltaY) {
	m_view.move(deltaX, deltaY);
	m_absoluteXView.move(0, deltaY);
	m_absoluteYView.move(deltaX, 0);
}

// scrolling: (working better)
void TextView::scroll() {

	sf::Vector2i coords = m_window.mapCoordsToPixel(m_cursor.getShape().getPosition(), m_view);
	sf::Vector2f newCoords = m_view.getCenter();

	// down scroll
	if (coords.y + Constants::FONT_SIZE > m_y + m_height) {
		std::cout << "scrolling down" << std::endl;
		newCoords.y = -m_height / 2 + (m_cursor.getLine() + 1) * Constants::FONT_SIZE;
		std::cout << "new coords /y: " << newCoords.y << std::endl;
	}

	// up scroll
	else if (coords.y < m_y) {
		std::cout << "scrolling up" << std::endl;
		newCoords.y = m_height / 2 + m_cursor.getLine() * Constants::FONT_SIZE;
		std::cout << "new coords /y: " << newCoords.y << std::endl;
	}

	int baseX = m_lineIndicator.getLocalBounds().width + Constants::FONT_WIDTH;

	// left scroll
	if (coords.x < m_x + baseX) {
		newCoords.x = m_width / 2 + m_cursor.getPos() * Constants::FONT_WIDTH;
		if (coords.x < m_width / 2 - baseX)	newCoords.x = m_width / 2;
	}

	// right scroll
	else if (coords.x + Constants::FONT_WIDTH > m_x + m_width) {
		std::cout << "scrolling right" << std::endl;
		newCoords.x = -m_width / 2 + baseX + (m_cursor.getPos() + 1) * Constants::FONT_WIDTH;
		std::cout << "new coords /x: " << newCoords.y << std::endl;
	}

	m_view.setCenter(newCoords.x, newCoords.y);
	m_absoluteXView.setCenter(m_absoluteXView.getCenter().x, newCoords.y);
	m_absoluteYView.setCenter(newCoords.x, m_absoluteYView.getCenter().y);
}

void TextView::clear() {
	m_lines = {
		Line("")
	};

	m_saveLocation = "";
	m_cursor.set(0, 0);
}

void TextView::save() {

	// remove the linted errors
	for (int i = 0; i < m_lines.size(); i++) {
		m_lines[i].resetIssues();
	}

	// if we don't know where to save this file, ask the user
	if (m_saveLocation.empty()) {

		// don't bother to save if we have an empty file
		if (m_lines.size() == 1 && m_lines[0].getText().empty()) return;

		nfdchar_t *outPath = nullptr;
		nfdresult_t result = NFD_SaveDialog(nullptr, nullptr, &outPath);

		if (result == NFD_OKAY) {

			std::ofstream file(outPath, std::ios::trunc);

			for (int i = 0; i < m_lines.size(); i++) {
				file << m_lines[i].getText() << "\n";
			}

			m_saveLocation = outPath;

			file.close();
			free(outPath);
		}
	}

	// now we should know where to save it, just check a last time, in case the prompt didn't succeed
	if (!m_saveLocation.empty()) {

		Project& project = Project::getCurrentProject();

		std::ofstream file(m_saveLocation, std::ios::trunc);

		for (int i = 0; i < m_lines.size(); i++) {
			file << m_lines[i].getText() << "\n";
		}

		file.close();

		// we are saving the project's dotfile, parse again the dotfile
		if (m_saveLocation == project.getDotFilePath()) {
			project.parseDotFile();
		}

		// LINT AGAIN !
		m_linter.lint();
		std::cout << "saving ..." << std::endl;
	}
}

void TextView::open() {

	nfdchar_t *inPath = nullptr;
	nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &inPath);

	if (result == NFD_OKAY) {

		// clear the textview
		clear();

		m_saveLocation = inPath;

		std::ifstream file(m_saveLocation);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();
		paste(stream);

		m_linter.lint();

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
	m_linter.lint();
}

std::string TextView::getSaveLocation() {
	return m_saveLocation;
}

TextView &TextView::getCurrentTextView() {
	return *m_textViews[m_currentTextViewIndex];
}

int TextView::getCurrentTextViewIndex() {
	return m_currentTextViewIndex;
}

void TextView::setCurrentTextViewIndex(int index) {
	m_textViews[m_currentTextViewIndex]->m_active = false;
	m_currentTextViewIndex = index;
	m_textViews[m_currentTextViewIndex]->m_active = true;
}

TextView& TextView::getTextView(int index)
{
	return *m_textViews[index];
}

int TextView::getTextViewNumber()
{
	return m_textViews.size();
}

void TextView::addTextView(TextView* textView) {
	m_textViews.push_back(textView);
	if (!m_textViews.empty()) m_textViews[m_currentTextViewIndex]->m_active = false;
	m_currentTextViewIndex = m_textViews.size() - 1;
	m_textViews[m_currentTextViewIndex]->m_active = true;
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

void TextView::setActive(bool active) {
	m_active = active;
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

void TextView::setRPos(sf::Vector2f rpos) {
	m_rx = rpos.x;
	m_ry = rpos.y;
}

void TextView::setRSize(sf::Vector2f rsize) {
	m_rwidth = rsize.x;
	m_rheight = rsize.y;
}

sf::Vector2f TextView::getRPos()
{
	return sf::Vector2f(m_rx, m_ry);
}

sf::Vector2f TextView::getRSize()
{
	return sf::Vector2f(m_rwidth, m_rheight);
}

Line& TextView::getLine(int i) {
	return m_lines[i];
}