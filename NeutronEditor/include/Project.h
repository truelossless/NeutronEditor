#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "ConsoleCommand.h"

class Project
{
public:
	Project();
	~Project();

	struct CompileTask {
		ConsoleCommand* command;
		std::string* output;
		std::filesystem::path buildPath;
	};

	void setPath(std::string path);
	std::string getPath();
	void createDotFile();
	bool checkForDotFile();
	bool empty();
	std::string getDotFilePath();

	bool parseDotFile();
	std::vector<std::string>& getIncludePath();

	std::string getLanguage() const;
	std::string getCompiler() const;

	void compile();
	void registerCompileTask(ConsoleCommand& compileCommand, std::filesystem::path buildPath);

	static void setCurrentProject(Project& project);
	static Project& getCurrentProject();
	static void runCompileTasks();

private:
	std::string m_path = "";

	std::string m_language;
	std::string m_compiler;
	std::string m_linterSettings;
	std::vector<std::string> m_includePath;
	std::vector<std::string> m_libPath;
	std::vector<std::string> m_libs;
	
	ConsoleCommand m_compileCommand;
	std::string m_compileOutput;

	static Project *m_currentProject;
	static std::vector<CompileTask> m_compileTasks;
};

