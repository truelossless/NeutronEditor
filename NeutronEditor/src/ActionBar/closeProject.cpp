#include "ActionBar.h"

#include "Project.h"

void ActionBar::action_closeProject(ActionBar& actionBar, TextView& textView) {

	Project& project = Project::getCurrentProject();
	project.setPath("");

	actionBar.setActive(false);
}