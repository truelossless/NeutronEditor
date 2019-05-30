#include "ActionBar.h"
#include <iostream>

void ActionBar::action_save(ActionBar& actionBar) {
	actionBar.m_currentTextView->save();
	actionBar.setActive(false);
}