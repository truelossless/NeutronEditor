#include "ActionBar.h"

void ActionBar::action_close(ActionBar& actionBar) {
	actionBar.m_currentTextView->save();
	actionBar.m_currentTextView->clear();
	actionBar.setActive(false);
}

void ActionBar::action_forceClose(ActionBar& actionBar) {
	actionBar.m_currentTextView->clear();
	actionBar.setActive(false);
}