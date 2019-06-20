#include "ActionBar.h"

#include <iostream>

#include "Project.h"

void ActionBar::action_openProjectDotFile(ActionBar& actionBar, TextView& textView) {
	
	Project& project = Project::getCurrentProject();
	
	if (!project.empty()) {
		textView.save();
		std::cout << project.getDotFilePath() << std::endl;
		textView.open(project.getDotFilePath());
	}
	else {
		actionBar.setNotif("No project opened !");
	}

	actionBar.setActive(false);
}