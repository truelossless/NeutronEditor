#include "ActionBar.h"

#include <filesystem>
#include <nfd.h>
#include <iostream>

#include "Project.h"

void ActionBar::action_openProject(ActionBar& actionBar, TextView& textView) {

	Project& project = Project::getCurrentProject();

	nfdchar_t *outPath = nullptr;
	nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

	if (result == NFD_OKAY) {

		// we must check the path to ensure that a .nproj file is present
		std::filesystem::path dotFile(outPath);
		dotFile /= ".nproj";

		if (project.getDotFilePath() == dotFile.u8string()) {
			actionBar.setNotif("Project already opened !");
		}
		else if (std::filesystem::exists(dotFile)) {
			project.setPath(outPath);
			actionBar.setNotif("Project opened !");
			project.parseDotFile();
		}
		else {
			actionBar.setNotif("No project to open here :(");
		}

		free(outPath);
	}

	actionBar.setActive(false);
}