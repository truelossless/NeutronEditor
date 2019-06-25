#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "Line.h"
#include "Cursor.h"
#include "Linter.h"

class ActionBar;

class TextView {
public:

	// take the coordinates as fractions
	TextView(sf::RenderWindow& window, ActionBar& actionBar, float rx, float ry, float rwidth, float rheight);
	~TextView();

	// basic operations
	void addLine(std::string line);
	void insertLine(int lineNum, Line& line);
	void insertLine(int lineNum);
	void deleteLine(int lineNum);
	int getLineCount();
	void setActive(bool active);

	Cursor* getCursor();
	Line& getLine(int i);

	// user-triggered operations
	int enterInsertLine(int charnum, int lineNum);
	int backspaceDeleteLine(int lineNum);

	void paste(std::string& text);
	void paste(std::stringstream& iss);

	void insert(char character, int charNum, int lineNum);
	void erase(int charNum, int lineNum);

	// size of the editor
	void setRPos(sf::Vector2f rpos);
	void setRSize(sf::Vector2f rsize);
	sf::Vector2f getRPos();
	sf::Vector2f getRSize();

	// drawing operations
	void draw();
	int getLineIndicatorNumberCount();

	// scrolling operations
	void updateView();
	void moveView(int deltaX, int deltaY);
	void scroll();

	// i/o operations
	void clear();
	void save();
	void open();
	void open(std::string path);
	std::string getSaveLocation();

	// static methods
	static TextView& getCurrentTextView();
	static int getCurrentTextViewIndex();
	static void setCurrentTextViewIndex(int index);
	static TextView& getTextView(int index);
	static int getTextViewNumber();
	static void addTextView(TextView* textView);

private:

	// all the textviews are available here
	static std::vector<TextView*> m_textViews;
	static int m_currentTextViewIndex;
	
	Linter m_linter;

	sf::RenderWindow& m_window;
	ActionBar& m_actionBar;
	std::vector<Line> m_lines;

	bool m_active = false;

	// main view
	sf::View m_view;
	sf::View m_absoluteView;
	sf::View m_absoluteXView;
	sf::View m_absoluteYView;

	sf::RectangleShape m_lineIndicator;

	Cursor m_cursor;

	int m_x;
	int m_y;
	int m_width;
	int m_height;

	// fraction coordinates
	float m_rx;
	float m_ry;
	float m_rwidth;
	float m_rheight;

	std::string m_saveLocation = "";
};

