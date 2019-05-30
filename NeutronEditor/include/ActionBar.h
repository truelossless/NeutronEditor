#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

#include "Cursor.h"
#include "TextView.h"

class ActionBar
{
public:

	struct Command {
		std::string desc;
		std::vector<char> shortcut;
		std::function<void(ActionBar&)> function;
	};

	ActionBar(sf::RenderWindow& window);
	~ActionBar();
	
	void draw();
	void updateView(int width, int height);

	void insert(char character);
	void erase();

	bool isActive() const;
	void setActive(bool active);
	std::string getText();
	void submit();

	// prompt the user to enter a value
	void setRequest(std::string request, std::function<void(std::string)> callback);

	void setCurrentTextView(TextView* textView);

	// all action bar commands
	static void action_close(ActionBar& actionBar);
	static void action_forceClose(ActionBar& actionBar);
	static void action_save(ActionBar& actionBar);
	static void action_open(ActionBar& actionBar);
	static void action_openThemeFile(ActionBar& actionBar);
	static void action_openConfFile(ActionBar& actionBar);
	static void action_reloadTheme(ActionBar& actionBar);
	static void action_setFontSize(ActionBar& actionBar);
	static void action_setFontSizeCallback(std::string text);

private:
	bool m_active = false;
	bool m_userRequester = false;

	sf::RectangleShape m_barShape;
	sf::RectangleShape m_autocompleteShape;
	sf::Text m_text;
	sf::RenderWindow& m_window;

	std::string m_requester;
	std::string m_command;
	const std::string m_splash = "Neutron editor v0.1 - SHIFT SPACE to get started :)";
	TextView* m_currentTextView;

	std::function<void(std::string)> m_callback;
	std::vector<Command> m_autoCompleteCommands;

	static const std::vector<Command> m_commands;

	void autoComplete();
};

