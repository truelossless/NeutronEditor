#include "ActionBar.h"

void ActionBar::action_openConfFile(ActionBar& actionBar) {

	actionBar.m_currentTextView->open("conf.json");
	actionBar.setActive(false);
}