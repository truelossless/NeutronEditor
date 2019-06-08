#include "ActionBar.h"

#include <iostream>

void ActionBar::action_open(ActionBar& actionBar, TextView& textView) {
	textView.open();
	actionBar.setActive(false);
}