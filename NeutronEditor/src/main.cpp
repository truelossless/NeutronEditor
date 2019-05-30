#include <nfd.h>
#include <json/json.h>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <fstream>

#include "Constants.h"
#include "InputHandler.h"
#include "TextView.h"
#include "ActionBar.h"
#include "SyntaxHighlighter.h"
#include "main.h"

int main() {

	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Neutron text editor", sf::Style::Default, settings);

	Constants::setAbsoluteView(window.getDefaultView());

	std::string fontBaseDir = "C:/Windows/Fonts/";

	Constants::setFont(fontBaseDir + "Hack-Regular.ttf");
	Constants::parseThemeFile();

	// instanciate a textview : basically a text editor.
// going with a class aspect will permit later to open multiple editors.
	TextView mainView(window);
	mainView.addLine("");

	ActionBar actionBar(window);
	actionBar.setCurrentTextView(&mainView);
	//actionBar.setActive(true);

	InputHandler inputHandler;

	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			else if (event.type == sf::Event::Resized) {
				// update views with the new window size
				Constants::setAbsoluteView(event.size.width, event.size.height);
				mainView.updateView(event.size.width, event.size.height);
				actionBar.updateView(event.size.width, event.size.height);
			}
			else {

				InputHandler::Result res;

				if (actionBar.isActive()) {
					res = inputHandler.handleActionBar(event, actionBar);
					if (res.focusChanged && res.newFocus == InputHandler::Focus::TextView) actionBar.setActive(false);
				}
				else {
					res = inputHandler.handleTextView(event, mainView);
					actionBar.setCurrentTextView(&mainView);
					if (res.focusChanged && res.newFocus == InputHandler::Focus::ActionBar) actionBar.setActive(true);
				}

			}
		}

		window.clear(Constants::BACKGROUND_COLOR);

		mainView.scroll();
		// draw the text view
		mainView.draw();

		// draw the action bar
		actionBar.draw();

		window.display();
	}

	return 0;
}