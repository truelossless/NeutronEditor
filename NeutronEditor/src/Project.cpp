#include "Project.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <cctype>

#include <json/json.h>

#include "TextView.h"


Project* Project::m_currentProject;
std::vector<Project::CompileTask> Project::m_compileTasks;

Project::Project() {
}


Project::~Project() {
}

void Project::setPath(std::string path) {
	m_path = path;
}

void Project::createDotFile() {

	if (m_path.empty()) return;

	std::filesystem::path dotFile(m_path);
	// append path operator
	dotFile /= ".nproj";

	// if the file doesn't exist, create it
	if (!std::filesystem::exists(dotFile)) {

		std::ofstream dotFileStream(dotFile);
		dotFileStream << "// Neutron project file\n{\n}";
		dotFileStream.close();
	}
}

bool Project::checkForDotFile() {
	std::filesystem::path dotFile(m_path);
	dotFile /= ".nproj";

	return std::filesystem::exists(dotFile);
}

bool Project::empty() {
	return m_path.empty();
}

std::string Project::getDotFilePath() {
	return std::filesystem::path(m_path).append(".nproj").u8string();
}

bool Project::parseDotFile() {

	// exit if there is no project
	if (m_path.empty()) return false;

	// parse the dotfile
	std::ifstream dotFile(getDotFilePath());
	Json::Value json;

	try {
		dotFile >> json;
	}
	catch (Json::RuntimeError err) {

		if (TextView::getCurrentTextView().getSaveLocation() == getDotFilePath()) {
			// there is no public api in our json library to get the line number of the error, which SUCKS :/
			// so I'll be parsing here the result from the error string ...

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

			TextView::getCurrentTextView().getLine(std::stoi(lineNumber) - 1).setErrored(true);
		}

		return false;
	}

	std::string str((std::istreambuf_iterator<char>(dotFile)), std::istreambuf_iterator<char>());

	dotFile.close();

	// and populate accordingly our object
	m_language = json.get("language", "").asString();
	m_compiler = json.get("compiler", "").asString();

	m_includePath.clear();

	if (json["includePath"].isArray()) {

		for (unsigned int i = 0; i < json["includePath"].size(); i++) {
			m_includePath.push_back(json["includePath"][i].asString());
		}
	}

	m_libPath.clear();

	if (json["libPath"].isArray()) {

		for (unsigned int i = 0; i < json["libPath"].size(); i++) {
			m_libPath.push_back(json["libPath"][i].asString());
		}
	}

	m_libs.clear();

	if (json["libs"].isArray()) {

		for (unsigned int i = 0; i < json["libs"].size(); i++) {
			m_libs.push_back(json["libs"][i].asString());
		}
	}

	return true;
}

std::string Project::getLanguage() const {
	return m_language;
}

std::string Project::getCompiler() const
{
	return m_compiler;
}

void Project::compile() {

	if (m_language.empty() || m_compiler.empty()) return;

	if (m_language == "cpp" || m_language == "c++") {


		// compile command
		std::string compileString;
		if (m_compiler == "gcc") compileString += "g++ ";
		else if (m_compiler == "clang") compileString += "clang++ ";
		else compileString += m_compiler + " ";

		std::filesystem::path mainPath = m_path;
		mainPath /= "main.cpp";

		// path of the main cpp file
		std::string compileString = "g++ " + mainPath.u8string() + " ";

		// output under the build folder
		std::filesystem::path buildPath = m_path;
		buildPath /= "build";

		// create the build directory
		std::filesystem::create_directories(buildPath);

		buildPath /= "program.exe";

		compileString += " -o " + buildPath.u8string();

		// include paths
		for (int i = 0; i < m_includePath.size(); i++) {
			compileString += " -I " + m_includePath[i];
		}

		// lib paths
		for (int i = 0; i < m_libPath.size(); i++) {
			compileString += " -L " + m_libPath[i];
		}

		// libs
		for (int i = 0; i < m_libs.size(); i++) {
			compileString += " -l" + m_libs[i];
		}

		std::cout << compileString << std::endl;
		m_compileCommand.set(compileString);
		registerCompileTask(m_compileCommand, buildPath);

	}

}

void Project::registerCompileTask(ConsoleCommand& compileCommand, std::filesystem::path buildPath) {
	std::cout << "registered task" << std::endl;
	CompileTask task = { &compileCommand, &m_compileOutput, buildPath };
	m_compileTasks.push_back(task);
}

void Project::setCurrentProject(Project& project) {
	m_currentProject = &project;
}

Project& Project::getCurrentProject() {
	return *m_currentProject;
}

void Project::runCompileTasks() {

	for (int i = m_compileTasks.size() - 1; i >= 0; i--) {
		std::cout << "compiling ..." << std::endl;

		if (m_compileTasks[i].command->running()) {
			ConsoleCommand::ConsoleCommandOutput compileOutput = m_compileTasks[i].command->run();

			if (compileOutput.finished) {
				*m_compileTasks[i].output += compileOutput.buffer;
				std::cout << m_compileTasks[i].output << std::endl;
			}
		}
		else {

			std::cout << "running da shit > " << m_compileTasks[i].buildPath.u8string() << std::endl;

			// the program doesn't have his own process so it won't be shown
			// i'll implement soon a cross-platform solution, by using CreateProcess on windows and fork on unix systems
			ConsoleCommand runProg;
			runProg.syncRun(m_compileTasks[i].buildPath.u8string());

			// delete the task
			m_compileTasks.erase(m_compileTasks.begin() + i);
		}

	}
}
