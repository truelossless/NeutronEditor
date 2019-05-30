#include "ActionBar.h"

#include <iostream>

void ActionBar::action_open(ActionBar& actionBar) {
	actionBar.m_currentTextView->open();
	actionBar.setActive(false);
}