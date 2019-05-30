#pragma once

#include "Constants.h"

class Cursor
{
public:
	Cursor();
	~Cursor();

	void up();
	void up(int i);
	void down();
	void down(int i);
	void left();
	void left(int i);
	void right();
	void right(int i);

	int getLine() const;
	int getPos() const;
	
	void setPos(int charNum);
	void setLine(int lineNum);
	void set(int charNum, int lineNum);

	sf::Shape& getShape();

private:
	sf::RectangleShape m_shape;
	int m_line = 0;
	int m_pos = 0;
};

