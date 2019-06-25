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
#include "Project.h"
#include "Linter.h"

int main() {

	sf::ContextSettings settings;
	//settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1200, 600), "Neutron text editor", sf::Style::Default, settings);
	Constants::setAbsoluteView(window.getDefaultView());

	std::string fontBaseDir = "C:/Windows/Fonts/";

	Constants::setFont(fontBaseDir + "Hack-Regular.ttf");
	Constants::parseThemeFile();
	
	ActionBar actionBar(window);

	TextView* mainTextView = new TextView(window, actionBar, 0, 0, 1, 1);
	TextView::addTextView(mainTextView);
	InputHandler inputHandler;

	Project project;
	Project::setCurrentProject(project);

	while (window.isOpen()) {
		
		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			else if (event.type == sf::Event::Resized) {
				// update views with the new window size
				Constants::setAbsoluteView(event.size.width, event.size.height);
				for (int i = 0; i < TextView::getTextViewNumber(); i++) {
					TextView::getTextView(i).updateView();
				}
				actionBar.updateView(event.size.width, event.size.height);
			}
			else {

				InputHandler::Result res;

				if (actionBar.isActive()) {
					res = inputHandler.handleActionBar(event, actionBar);
					if (res.focusChanged && res.newFocus == InputHandler::Focus::TextView) actionBar.setActive(false);
				}
				else {
					res = inputHandler.handleTextView(event);
					if (res.focusChanged && res.newFocus == InputHandler::Focus::ActionBar) actionBar.setActive(true);
				}

			}
		}

		ConsoleCommand::AsyncManager::runCommands();
		Project::runCompileTasks();

		window.clear(Constants::BACKGROUND_COLOR);

		// check if scrolling is needed for our current text view
		TextView::getCurrentTextView().scroll();
		
		// draw the textviews
		for (int i = 0; i < TextView::getTextViewNumber(); i++) {
			TextView::getTextView(i).draw();
		}
		
		// draw the action bar
		actionBar.draw();

		window.display();
	}

	return 0;
}