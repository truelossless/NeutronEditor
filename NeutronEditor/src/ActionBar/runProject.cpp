#include "ActionBar.h"

#include "Project.h"

void ActionBar::action_runProject(ActionBar& actionBar, TextView& textView) {

	Project& project = Project::getCurrentProject();

	if (!project.empty()) {

		if (project.getLanguage().empty()) {
			actionBar.setNotif("No language specified in your .nproj !");
		}
		
		else if (project.getCompiler().empty()) {
			actionBar.setNotif("No compiler specified in your .nproj !");
		}

		else {
			actionBar.setNotif("Compilation started ...");
		}

		project.compile();
	}
	else {
		actionBar.setNotif("No project opened !");
	}

	actionBar.setActive(false);
}