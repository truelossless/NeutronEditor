#include "ConsoleCommand.h"
#include "Linter.h"
#include <iostream>

Linter::Linter() {
}


Linter::~Linter()
{
}

bool Linter::executableInPath() {

	ConsoleCommand clangVersion;
	std::string res = clangVersion.syncRun("clang-tidy --version");

	// if we get a "version" string in the result, clang tidy is installed
	return res.find("version") != std::string::npos;
}
