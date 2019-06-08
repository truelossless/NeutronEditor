#include "ActionBar.h"

#include "Constants.h"

void ActionBar::action_reloadTheme(ActionBar& actionBar, TextView& textView) {
	
	Constants::parseThemeFile();
	actionBar.setActive(false);
}