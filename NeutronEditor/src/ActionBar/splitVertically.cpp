#include "ActionBar.h"

#include "TextView.h"

void ActionBar::action_splitVertically(ActionBar& actionBar, TextView& textView) {
	
	sf::Vector2f size = textView.getRSize();
	sf::Vector2f pos = textView.getRPos();
	
	size.x /= 2;
	textView.setRSize(size);
	textView.updateView();

	TextView::addTextView(TextView(actionBar.m_window, pos.x + size.x, pos.y, size.x, size.y));
	actionBar.setActive(false);
}