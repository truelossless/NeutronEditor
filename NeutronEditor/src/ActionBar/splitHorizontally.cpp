#include "ActionBar.h"

void ActionBar::action_splitHorizontally(ActionBar& actionBar, TextView& textView) {
	
	sf::Vector2f size = textView.getRSize();
	sf::Vector2f pos = textView.getRPos();

	size.y /= 2;
	textView.setRSize(size);
	textView.updateView();

	TextView* newTextView = new TextView(actionBar.m_window, actionBar, pos.x, pos.y + size.y, size.x, size.y);

	TextView::addTextView(newTextView);
	TextView::setCurrentTextViewIndex(TextView::getTextViewNumber() - 1);
	actionBar.setActive(false);
}