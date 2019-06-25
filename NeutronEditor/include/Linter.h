#pragma once

#include <string>
#include <vector>

#include "ConsoleCommand.h"

class TextView;

class Linter {

public:

	enum class IssueLevel {
		Error,
		Warning
	};

	struct LintIssue {
		// negative number: we don't have data about this field
		int line = -1;
		int column = -1;
		IssueLevel issueLevel;
		std::string error;
	};

	Linter(TextView& textView);
	~Linter();

	// auto detect the language based on the file extension
	void lint();
	
	void jsonLint();
	void cppLint();
	bool clangTidyInPath();

private:
	std::string m_fileExtension;
	std::string m_saveLocation;
	ConsoleCommand m_lintCommand;
	TextView& m_textView;

	std::string m_lintOutput;
	
	std::vector<LintIssue> m_issues;

	void parseClangTidyOutput();
	void checkFile();

	static bool m_clangTidySearched;
	static bool m_clangTidyInPath;
};

