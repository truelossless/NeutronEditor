#pragma once

#include <string>
#include <vector>
#include <future>
#include <functional>

class ConsoleCommand
{
public:

	struct ConsoleCommandOutput {
		bool finished = false;
		std::string buffer = "";
	};

	struct ConsoleCommandTask {
		ConsoleCommand* command;
		std::string& output;
		std::function<void()> callback;
	};

	ConsoleCommand();
	~ConsoleCommand();

	// ... run asynchronously
	void set(std::string command);
	bool running();
	ConsoleCommandOutput run();

	// or run synchronously
	std::string syncRun(std::string command);

	// async manager, to run the commands and provide a callback
	class AsyncManager {

	public:
		static void registerCommand(ConsoleCommandTask* task);
		static void runCommands();

	private:
		static std::vector<ConsoleCommandTask*> m_tasks;
	};

private:
	std::string m_output;
	bool m_running= false;
	FILE* m_pipe = nullptr;

	FILE* open(std::string command);
	void close(FILE* pipe);
	ConsoleCommandOutput process();
	std::future<ConsoleCommandOutput> m_future;
};

