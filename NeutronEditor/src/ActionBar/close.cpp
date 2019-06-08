#include "ActionBar.h"

void ActionBar::action_close(ActionBar& actionBar, TextView& textView) {
	textView.save();
	textView.clear();
	actionBar.setActive(false);
}

void ActionBar::action_forceClose(ActionBar& actionBar, TextView& textView) {
	textView.clear();
	actionBar.setActive(false);
}