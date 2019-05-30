#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "Line.h"
#include "Cursor.h"

class TextView
{
public:
	TextView(sf::RenderWindow& window);
	~TextView();
	
	// basic operations
	void addLine(std::string line);
	void insertLine(int lineNum, Line& line);
	void insertLine(int lineNum);
	void deleteLine(int lineNum);
	int getLineCount();

	Cursor* getCursor();
	Line& getLine(int i);

	// editor size & position
	void setPosition(int x, int y);
	void setSize(int width, int height);

	// user-triggered operations
	int enterInsertLine(int charnum, int lineNum);
	int backspaceDeleteLine(int lineNum);

	void paste(std::string& text);
	void paste(std::stringstream& iss);

	void insert(char character, int charNum, int lineNum);
	void erase(int charNum, int lineNum);
	
	// drawing operations
	void draw();
	int getLineIndicatorNumberCount();

	// scrolling operations
	void updateView(int width, int height);
	void moveView(int deltaX, int deltaY);
	void scroll();

	// i/o operations
	void clear();
	void save();
	void open();
	void open(std::string path);

private:
	sf::RenderWindow& m_window;
	std::vector<Line> m_lines;

	// main view
	sf::View m_view;
	sf::View m_absoluteXView;
	sf::View m_absoluteYView;
	
	sf::RectangleShape m_lineIndicator;

	Cursor m_cursor;
	
	int m_x = 0;
	int m_y = 0;
	int m_width;
	int m_height;

	std::string m_saveLocation;
};

