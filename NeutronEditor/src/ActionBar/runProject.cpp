#include "ActionBar.h"

#include "Project.h"

void ActionBar::action_runProject(ActionBar& actionBar, TextView& textView) {

	Project& project = Project::getCurrentProject();

	if (!project.empty()) {
		actionBar.setNotif("Compilation started ...");
		project.compile();
	}
	else {
		actionBar.setNotif("No project opened !");
	}

	actionBar.setActive(false);
}