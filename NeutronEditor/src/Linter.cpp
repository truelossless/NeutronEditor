#include "Linter.h"

#include <iostream>
#include <filesystem>
#include <cctype>
#include <regex>
#include <fstream>
#include <json/json.h>

#include "Project.h"
#include "TextView.h"

bool Linter::m_clangTidyInPath = false;
bool Linter::m_clangTidySearched = false;

Linter::Linter(TextView& textView) :
	m_textView(textView) {
}

Linter::~Linter()
{
}

void Linter::lint() {

	m_lintOutput = "";

	m_saveLocation = m_textView.getSaveLocation();

	// std::filesystem extension() returns an empty string for dotfiles
	std::string fileName = std::filesystem::path(m_saveLocation).filename().u8string();

	if (fileName[0] == '.') {
		m_fileExtension = fileName;
	}
	else {
		m_fileExtension = std::filesystem::path(m_saveLocation).extension().u8string();
	}


	std::cout << m_textView.getSaveLocation();
	std::cout << "ext: " << m_fileExtension << std::endl;
	m_issues.clear();

	if (m_fileExtension == ".cpp" || m_fileExtension == ".cc" || m_fileExtension == ".c++") cppLint();
	else if (m_fileExtension == ".nproj" || m_fileExtension == ".json") jsonLint();

}

void Linter::jsonLint() {

	// there is no public api in our json library to get the line number of the error, which SUCKS :/
	// so I'll be parsing here the result from the error string ...

	std::ifstream jsonFile(m_textView.getSaveLocation());
	Json::Value json;

	try {
		jsonFile >> json;
	}
	catch (Json::RuntimeError err) {

		std::string errorMessage = std::string(err.what());

		std::string firstLine = errorMessage.substr(7);
		std::string lineNumber;
		std::string::iterator it = firstLine.begin();

		while (it != firstLine.end() && std::isdigit(*it)) {
			lineNumber += *it;
			++it;
		}

		size_t secondLinePos = errorMessage.find('\n');
		std::string secondLine = errorMessage.substr(secondLinePos + 3);

		m_issues.push_back({
			std::stoi(lineNumber),
			-1,
			IssueLevel::Error,
			secondLine
			});
		
		std::cout << secondLine << std::endl;
		m_textView.getLine(std::stoi(lineNumber) - 1).setErrored(true);
		m_textView.getLine(std::stoi(lineNumber) - 1).setIssue(secondLine);
	}


}

void Linter::cppLint() {

	Project& project = Project::getCurrentProject();

	if (!clangTidyInPath()) {
		//actionBar.setNotif("clang-tidy (c++ linter) not found !");
		return;
	}

	std::string lintString = "clang-tidy -quiet ";

	lintString += m_saveLocation;
	lintString += " -- ";

	std::vector<std::string> includePath = project.getIncludePath();

	for (int i = 0; i < includePath.size(); i++) {
		lintString += "-I";
		lintString += includePath[i];
	}

	std::cout << "lint command: " << lintString << std::endl;
	m_lintCommand.set(lintString);

	ConsoleCommand::ConsoleCommandTask* task = new ConsoleCommand::ConsoleCommandTask {
		&m_lintCommand,
		m_lintOutput,
		std::bind(&Linter::parseClangTidyOutput, this)
	};

	ConsoleCommand::AsyncManager::registerCommand(task);
}

bool Linter::clangTidyInPath() {

	if (!m_clangTidySearched) {
		ConsoleCommand clangVersion;
		std::string res = clangVersion.syncRun("clang-tidy --version");

		// if we get a "version" string in the result, clang tidy is installed
		bool inPath = res.find("version") != std::string::npos;

		m_clangTidyInPath = inPath;
		m_clangTidySearched = true;

		return inPath;
	}
	else {
		return m_clangTidyInPath;
	}

}

void Linter::parseClangTidyOutput() {
	// if we're here, m_output is populated with the result of clang-tidy.

	// parse clang-tidy output :(
	// because fucking clang-tidy hasn't any api to get the result >:(

	std::cout << "parsing output ..." << std::endl;

	std::smatch lintMatch;
	std::regex regex(":([0-9]+):([0-9]+): (warning|error): ([^\n]*)");

	std::string::const_iterator searchStart(m_lintOutput.cbegin());

	while (std::regex_search(searchStart, m_lintOutput.cend(), lintMatch, regex)) {

		// get all matches, omit the global match
		int line = std::stoi(lintMatch[1]);
		int column = std::stoi(lintMatch[2]);

		// wether it is an error or a warning
		bool error = lintMatch[3] == "error";

		std::string text = lintMatch[4];

		m_issues.push_back({
			line,
			column,
			error ? IssueLevel::Error : IssueLevel::Warning,
			text
			});

		if (error) {
			m_textView.getLine(line - 1).setErrored(true);
		}
		else {
			m_textView.getLine(line - 1).setWarned(true);
		}

		std::cout << "line " << line << " errored";
		m_textView.getLine(line - 1).setIssue(text);
		searchStart = lintMatch.suffix().first;
	}
}
