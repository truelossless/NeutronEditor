#include "Constants.h"

#include <SFML/Graphics.hpp>
#include <json/json.h>
#include <fstream>
#include <iostream>

int Constants::FONT_WIDTH;
sf::Font Constants::FONT;

sf::Color Constants::BACKGROUND_COLOR;
sf::Color Constants::TEXT_COLOR;
sf::Color Constants::QUOTES_COLOR;
sf::Color Constants::COMMENT_COLOR;
sf::Color Constants::OPERATOR_COLOR;
sf::Color Constants::TYPE_COLOR;
sf::Color Constants::UNDER_CURSOR_COLOR;
sf::Color Constants::CURRENT_LINE_COLOR;
sf::Color Constants::CURSOR_COLOR;
sf::Color Constants::NUMBER_COLOR;

sf::Color Constants::LINE_INDICATOR_TEXT_COLOR;
sf::Color Constants::LINE_INDICATOR_BACKGROUND_COLOR;

sf::Color Constants::ACTIONBAR_COLOR;
sf::Color Constants::ACTIONBAR_BORDER_COLOR;
int Constants::ACTIONBAR_BORDER_WIDTH;
sf::Color Constants::ACTIONBAR_INACTIVE_TEXT_COLOR;
sf::Color Constants::ACTIONBAR_ACTIVE_TEXT_COLOR;

sf::View Constants::absoluteView;

void Constants::parseThemeFile() {

	std::ifstream theme("theme.json");
	Json::Value json;
	theme >> json;

	theme.close();

	// retreive all the values from the json theme file
	BACKGROUND_COLOR = parseColor(json["editor"]["backgroundColor"]);
	TEXT_COLOR = parseColor(json["editor"]["textColor"]);
	QUOTES_COLOR = parseColor(json["editor"]["quotesColor"]);
	COMMENT_COLOR = parseColor(json["editor"]["commentsColor"]);
	OPERATOR_COLOR = parseColor(json["editor"]["operatorColor"]);
	TYPE_COLOR = parseColor(json["editor"]["typeColor"]);
	UNDER_CURSOR_COLOR = parseColor(json["editor"]["underCursorColor"]);
	CURRENT_LINE_COLOR = parseColor(json["editor"]["currentLineColor"]);
	CURSOR_COLOR = parseColor(json["editor"]["cursorColor"]);
	NUMBER_COLOR = parseColor(json["editor"]["numberColor"]);
	
	LINE_INDICATOR_BACKGROUND_COLOR = parseColor(json["lineIndicator"]["backgroundColor"]);
	LINE_INDICATOR_TEXT_COLOR = parseColor(json["lineIndicator"]["textColor"]);

	ACTIONBAR_COLOR = parseColor(json["actionBar"]["backgroundColor"]);
	ACTIONBAR_INACTIVE_TEXT_COLOR = parseColor(json["actionBar"]["inactiveTextColor"]);
	ACTIONBAR_ACTIVE_TEXT_COLOR = parseColor(json["actionBar"]["activeTextColor"]);
	ACTIONBAR_BORDER_COLOR = parseColor(json["actionBar"]["border"]["color"]);
	ACTIONBAR_BORDER_WIDTH = json["actionBar"]["border"].get("width", 1).asInt();
}

void Constants::setFont(std::string font) {
	FONT.loadFromFile(font);
	sf::Text char1("_", FONT, FONT_SIZE);
	FONT_WIDTH = static_cast<unsigned int>(char1.getLocalBounds().width);
}

void Constants::setAbsoluteView(sf::View view) {
	absoluteView = view;
}

void Constants::setAbsoluteView(int width, int height)
{
	absoluteView = sf::View(sf::FloatRect(0, 0, static_cast<float>(width), static_cast<float>(height)));
	// set back the center of our view.
	// we must round the values because if our view is not pixel perfect, the text is going to be blurry.
	absoluteView.setCenter(static_cast<int>(width / 2), static_cast<int>(height / 2));
}

sf::Color Constants::parseColor(Json::Value color)
{
	int r = color.get("r", 255).asInt();
	int g = color.get("g", 255).asInt();
	int b = color.get("b", 255).asInt();
	int a = color.get("a", 255).asInt();
	return sf::Color(r, g, b, a);
}
