#include "ActionBar.h"

// be sure to use a c++17-compliant compiler
#include <filesystem>
#include <nfd.h>

#include "Project.h"

void ActionBar::action_createProject(ActionBar& actionBar, TextView& textView) {

	Project& project = Project::getCurrentProject();

	// check if there is a project file based on the current file save location
	if (!textView.getSaveLocation().empty()) {
		
		std::filesystem::path currentFile = textView.getSaveLocation();

		if (currentFile.has_parent_path()) {
			project.setPath(currentFile.parent_path().u8string());
		}
	}
	// else let the user choose his directory
	else {
		nfdchar_t *outPath = nullptr;
		nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

		if (result == NFD_OKAY) {
			project.setPath(outPath);
			free(outPath);
		}
	}
	
	// finally create the dotfile
	if (project.checkForDotFile()) {
		actionBar.setNotif("Project already created !");
	}
	else {
		project.createDotFile();
		actionBar.setNotif("Project created !");
	}

	actionBar.setActive(false);
}