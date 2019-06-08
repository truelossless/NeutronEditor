#include "ActionBar.h"
#include <iostream>

void ActionBar::action_save(ActionBar& actionBar, TextView& textView) {
	textView.save();
	actionBar.setActive(false);
}