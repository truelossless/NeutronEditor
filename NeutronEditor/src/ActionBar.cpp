#include "ActionBar.h"

#include <iostream>

#include "Constants.h"

ActionBar::ActionBar(sf::RenderWindow& window):
	m_window(window) {

	m_command = m_splash;

	int width = window.getSize().x;
	int height = window.getSize().y;

	m_barShape.setSize(sf::Vector2f(width, Constants::FONT_SIZE*1.5));
	m_barShape.setPosition(0, height - Constants::FONT_SIZE*1.5);
	m_barShape.setFillColor(Constants::BACKGROUND_COLOR);
	m_barShape.setOutlineThickness(1);
	m_barShape.setOutlineColor(Constants::ACTIONBAR_BORDER_COLOR);
	
	m_text.setFont(Constants::FONT);
	m_text.setFillColor(Constants::ACTIONBAR_INACTIVE_TEXT_COLOR);
	m_text.setCharacterSize(Constants::FONT_SIZE);
	m_text.setString(m_command);
	m_text.setPosition(Constants::FONT_WIDTH, height - Constants::FONT_SIZE*1.25);

	m_autocompleteShape.setFillColor(Constants::ACTIONBAR_BORDER_COLOR);
}

void ActionBar::draw() {

	m_window.setView(Constants::absoluteView);
	
	int width = m_window.getSize().x;
	int height = m_window.getSize().y;

	if (!m_autoCompleteCommands.empty() && !m_userRequester) {
		int autoCompleteBgSize = (m_autoCompleteCommands.size()*1.2 + 3)*Constants::FONT_SIZE;

		m_autocompleteShape.setSize(sf::Vector2f(width, autoCompleteBgSize));
		m_autocompleteShape.setPosition(0, height - autoCompleteBgSize);
		m_window.draw(m_autocompleteShape);

		for (int i = 0; i < m_autoCompleteCommands.size(); i++) {

			std::string shortcutString;

			int fillerLength = 8 - (m_autoCompleteCommands[i].shortcut.size()*2);

			for (int j = 0; j < m_autoCompleteCommands[i].shortcut.size(); j++) {
				shortcutString += m_autoCompleteCommands[i].shortcut[j];
				shortcutString += " ";
			}


			for (int j = 0; j < fillerLength; j++) {
				shortcutString += " ";
			}

			shortcutString += "| ";

			sf::Text commandText;
			commandText.setFont(Constants::FONT);
			commandText.setCharacterSize(Constants::FONT_SIZE);
			commandText.setString(shortcutString + m_autoCompleteCommands[i].desc);
			commandText.setPosition(Constants::FONT_WIDTH*3, height - Constants::FONT_SIZE*(i*1.2+3.5));
			m_window.draw(commandText);
		}
	}

	m_window.draw(m_barShape);
	m_window.draw(m_text);

}

void ActionBar::updateView(int width, int height) {
	m_barShape.setSize(sf::Vector2f(width, Constants::FONT_SIZE*1.5));
	m_barShape.setPosition(0, height - Constants::FONT_SIZE*1.5);
	m_text.setPosition(Constants::FONT_WIDTH, height - Constants::FONT_SIZE*1.25);
}

void ActionBar::autoComplete() {

	m_autoCompleteCommands.clear();

	if (m_command[0] == ':' && m_command.length() > 1) {
		for (int i = 0; i < m_commands.size(); i++) {
			if (m_commands[i].desc.find(m_command.substr(1)) != std::string::npos) {
				m_autoCompleteCommands.push_back(m_commands[i]);
			}
		}
	}
	else if(m_command.length() > 0) {
		for (int i = 0; i < m_command.size(); i++) {
			
			if (m_command.length() > m_commands[i].shortcut.size()) continue;

			bool match = true;
			
			for (int j = 0; j < m_command.length(); j++) {
				if (m_command[j] != m_commands[i].shortcut[j]) {
					match = false;
					break;
				}
			}

			if (match) m_autoCompleteCommands.push_back(m_commands[i]);
		}
	}
}

void ActionBar::insert(char character) {

	m_command += character;

	m_text.setString(m_requester + m_command);
	
	autoComplete();
}

void ActionBar::erase() {
	m_command.pop_back();
	m_text.setString(m_requester + m_command);

	autoComplete();
}

bool ActionBar::isActive() const {
	return m_active;
}

void ActionBar::setActive(bool active) {
	m_active = active;

	if (m_active) {
		m_command = "";
		m_text.setString(m_command);
		m_barShape.setFillColor(Constants::ACTIONBAR_COLOR);
		m_text.setFillColor(Constants::ACTIONBAR_ACTIVE_TEXT_COLOR);
	}
	else {
		m_barShape.setFillColor(Constants::BACKGROUND_COLOR);
		m_text.setFillColor(Constants::ACTIONBAR_INACTIVE_TEXT_COLOR);
		m_command = m_splash;
		m_requester = "";
		m_text.setString(m_command);
		m_autoCompleteCommands.clear();
		m_userRequester = false;
	}
}

std::string ActionBar::getText()
{
	return m_command;
}

void ActionBar::submit()
{

	// the user is answering a requester
	if (m_userRequester && !m_command.empty()) {
		m_callback(m_command);
		setActive(false);

	// the user submits a command
	} else if(m_autoCompleteCommands.size() == 1){
		m_autoCompleteCommands[0].function(*this);
		m_autoCompleteCommands.clear();
	}
}

void ActionBar::setRequest(std::string request, std::function<void(std::string)> callback) {

	m_userRequester = true;
	m_command = "";
	m_requester = request;
	m_callback = callback;

	m_text.setString(m_requester + m_command);
}

void ActionBar::setCurrentTextView(TextView* textView) {
	m_currentTextView = textView;
}

ActionBar::~ActionBar()
{
}
