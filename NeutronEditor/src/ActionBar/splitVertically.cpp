#include "ActionBar.h"

#include "TextView.h"

void ActionBar::action_splitVertically(ActionBar& actionBar, TextView& textView) {
	
	sf::Vector2f size = textView.getRSize();
	sf::Vector2f pos = textView.getRPos();
	
	size.x /= 2;
	textView.setRSize(size);
	textView.updateView();

	TextView* newTextView = new TextView(actionBar.m_window, actionBar, pos.x + size.x, pos.y, size.x, size.y);

	TextView::addTextView(newTextView);
	actionBar.setActive(false);
}