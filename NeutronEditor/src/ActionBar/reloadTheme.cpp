#include "ActionBar.h"

#include "Constants.h"

void ActionBar::action_reloadTheme(ActionBar& actionBar) {
	
	Constants::parseThemeFile();
	actionBar.setActive(false);
}