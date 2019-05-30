#include "Cursor.h"

Cursor::Cursor():
	m_shape(sf::Vector2f(Constants::FONT_WIDTH + Constants::CURSOR_PADDING, Constants::FONT_SIZE + Constants::CURSOR_PADDING)) {
	
	m_shape.setOrigin(m_shape.getOrigin().x + Constants::CURSOR_PADDING / 2, m_shape.getOrigin().y + Constants::CURSOR_PADDING / 2);
	m_shape.setFillColor(Constants::CURSOR_COLOR);
}

Cursor::~Cursor() {
}

void Cursor::up() {
	m_line--;
}

void Cursor::up(int i) {
	m_line-=i;
}

void Cursor::down() {
	m_line++;
}

void Cursor::down(int i) {
	m_line+=i;
}

void Cursor::left() {
	m_pos--;
}

void Cursor::left(int i) {
	m_pos-=i;
}

void Cursor::right() {
	m_pos++;
}

void Cursor::right(int i) {
	m_pos+=i;
}

int Cursor::getLine() const
{
	return m_line;
}

int Cursor::getPos() const {
	return m_pos;
}

void Cursor::setPos(int charNum) {
	m_pos = charNum;
}

void Cursor::setLine(int lineNum) {
	m_line = lineNum;
}

void Cursor::set(int charNum, int lineNum) {
	m_line = lineNum;
	m_pos = charNum;
}

sf::Shape& Cursor::getShape() {
	return m_shape;
}
