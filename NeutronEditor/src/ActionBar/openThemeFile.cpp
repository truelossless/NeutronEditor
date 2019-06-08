#include "ActionBar.h"

void ActionBar::action_openThemeFile(ActionBar& actionBar, TextView& textView) {
	
	textView.open("theme.json");
	actionBar.setActive(false);
}