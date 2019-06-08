#include "ActionBar.h"

void ActionBar::action_openConfFile(ActionBar& actionBar, TextView& textView) {

	textView.open("conf.json");
	actionBar.setActive(false);
}