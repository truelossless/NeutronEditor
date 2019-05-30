#include "ActionBar.h"

void ActionBar::action_openThemeFile(ActionBar& actionBar) {
	
	actionBar.m_currentTextView->open("theme.json");
	actionBar.setActive(false);
}