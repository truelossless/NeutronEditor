#pragma once

#include <SFML/Graphics.hpp>
#include <json/json.h>

class Constants
{
public:
	static const int FONT_SIZE = 18;
	static const int CURSOR_PADDING = 2;
	static int FONT_WIDTH;
	static sf::Font FONT;

	static sf::Color BACKGROUND_COLOR;
	static sf::Color TEXT_COLOR;
	static sf::Color QUOTES_COLOR;
	static sf::Color COMMENT_COLOR;
	static sf::Color OPERATOR_COLOR;
	static sf::Color TYPE_COLOR;
	static sf::Color UNDER_CURSOR_COLOR;
	static sf::Color CURSOR_COLOR;
	static sf::Color NUMBER_COLOR;
	static sf::Color CURRENT_LINE_COLOR;
	
	static sf::Color LINE_INDICATOR_TEXT_COLOR;
	static sf::Color LINE_INDICATOR_BACKGROUND_COLOR;

	static sf::Color ACTIONBAR_BORDER_COLOR;
	static int ACTIONBAR_BORDER_WIDTH;
	static sf::Color ACTIONBAR_COLOR;
	static sf::Color ACTIONBAR_INACTIVE_TEXT_COLOR;
	static sf::Color ACTIONBAR_ACTIVE_TEXT_COLOR;

	static void parseThemeFile();
	static void setFont(std::string font);
	static void setAbsoluteView(sf::View view);
	static void setAbsoluteView(int width, int height);

	// absolute view (used especially for gui rendering);
	static sf::View absoluteView;

private:
	static sf::Color parseColor(Json::Value color);
};

