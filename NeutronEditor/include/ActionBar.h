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
		std::function<void(ActionBar&, TextView&)> function;
	};

	ActionBar(sf::RenderWindow& window);
	
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

	// all action bar commands

	// file
	static void action_close(ActionBar& actionBar, TextView& textView);
	static void action_forceClose(ActionBar& actionBar, TextView& textView);
	static void action_save(ActionBar& actionBar, TextView& textView);
	static void action_open(ActionBar& actionBar, TextView& textView);
	
	// settings
	static void action_openThemeFile(ActionBar& actionBar, TextView& textView);
	static void action_openConfFile(ActionBar& actionBar, TextView& textView);
	static void action_reloadTheme(ActionBar& actionBar, TextView& textView);
	static void action_setFontSize(ActionBar& actionBar, TextView& textView);
	static void action_setFontSizeCallback(std::string text);

	// window
	static void action_splitHorizontally(ActionBar& actionBar, TextView& textView);
	static void action_splitVertically(ActionBar& actionBar, TextView& textView);
	static void action_mergeHorizontally(ActionBar& actionBar, TextView& textView);
	static void action_mergeVertically(ActionBar& actionBar, TextView& textView);

private:
	bool m_active = false;
	bool m_userRequester = false;

	sf::RectangleShape m_barShape;
	sf::RectangleShape m_autocompleteShape;
	sf::RectangleShape m_terminal;
	sf::Text m_text;
	sf::RenderWindow& m_window;

	std::string m_requester;
	std::string m_command;
	const std::string m_splash = "Neutron editor v0.1 - SHIFT SPACE to get started :)";

	std::function<void(std::string)> m_callback;
	std::vector<Command> m_autoCompleteCommands;

	static const std::vector<Command> m_commands;

	void autoComplete();
};

