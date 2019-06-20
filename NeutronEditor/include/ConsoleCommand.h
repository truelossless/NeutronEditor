#pragma once

#include <string>
#include <future>

class ConsoleCommand
{
public:

	struct ConsoleCommandOutput {
		bool finished = false;
		std::string buffer = "";
	};

	ConsoleCommand();
	~ConsoleCommand();

	// ... run asynchronously
	void set(std::string command);
	bool running();
	ConsoleCommandOutput run();

	// or run synchronously
	std::string syncRun(std::string command);

private:
	std::string m_output;
	bool m_running= false;
	FILE* m_pipe = nullptr;

	FILE* open(std::string command);
	void close(FILE* pipe);
	ConsoleCommandOutput process();
	std::future<ConsoleCommandOutput> m_future;
};

