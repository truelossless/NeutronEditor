#include "ConsoleCommand.h"

#include <iostream>
#include <cstdio>

std::vector<ConsoleCommand::ConsoleCommandTask*> ConsoleCommand::AsyncManager::m_tasks;

ConsoleCommand::ConsoleCommand() {
}


ConsoleCommand::~ConsoleCommand() {
}

void ConsoleCommand::set(std::string command) {
	
	close(m_pipe);
	m_pipe = open(command);

	if (!m_pipe) return;

	m_running = true;

	// to not make the program hang, we are going to listen to the console in a separate thread.
	m_future = std::async(std::launch::async, &ConsoleCommand::process, this);
}

bool ConsoleCommand::running() {
	return m_running;
}

ConsoleCommand::ConsoleCommandOutput ConsoleCommand::run() {

	ConsoleCommandOutput ret;
	
	if (m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

		ConsoleCommandOutput futureRes = m_future.get();

		if (futureRes.finished) {
			// the command has returned

			// here finished means "we have new text to read".
			ret.finished = false;
			m_running = false;
	
		} else {

			ret.buffer = futureRes.buffer;
			ret.finished = true;
			// we still have stuff to listen, start a new async task
			m_future = std::async(std::launch::async, &ConsoleCommand::process, this);
		}
	}
	
	return ret;
}

std::string ConsoleCommand::syncRun(std::string command) {
	
	std::string result;
	char buffer[512];
	
	close(m_pipe);
	m_pipe = open(command);

	while (fgets(buffer, sizeof buffer, m_pipe) != NULL) {
		result += buffer;
	}

	return result;
}

// cross platform popen wrapper
FILE* ConsoleCommand::open(std::string command)
{
	#ifdef _WIN32
		return _popen(command.c_str(), "r");
	#else
		return popen(command.c_str(), "r");
	#endif
}

void ConsoleCommand::close(FILE* pipe) {

	// not sure if this line is needed because deleting a nullptr has no effect,
	// but we don't know the implementation of pclose.
	if (m_pipe == nullptr) return;

	#ifdef  _WIN32
		_pclose(pipe);
	#else
		pclose(pipe);
	#endif
}

ConsoleCommand::ConsoleCommandOutput ConsoleCommand::process() {

	char buffer[512];
	ConsoleCommandOutput ret;

	if (fgets(buffer, sizeof buffer, m_pipe) != NULL) {
		ret.buffer = buffer;
	}
	else {
		ret.finished = true;
	}

	return ret;
}

void ConsoleCommand::AsyncManager::registerCommand(ConsoleCommandTask* task) {
	m_tasks.push_back(task);
}

void ConsoleCommand::AsyncManager::runCommands() {

	for (int i = m_tasks.size() - 1; i >= 0; i--) {

		std::cout << "running commands" << std::endl;

		if (m_tasks[i]->command->running()) {
			ConsoleCommand::ConsoleCommandOutput compileOutput = m_tasks[i]->command->run();

			if (compileOutput.finished) {
				m_tasks[i]->output += compileOutput.buffer;
				std::cout << m_tasks[i]->output << std::endl;
			}
		}
		else {
			std::cout << "task finsihed, starting callback ..." << std::endl;
			m_tasks[i]->callback();

			delete m_tasks[i];
			m_tasks.erase(m_tasks.begin() + i);
			std::cout << "deleted task !" << std::endl;
		}
	}
}
