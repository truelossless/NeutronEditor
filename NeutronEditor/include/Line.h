#pragma once

#include <vector>

#include "Character.h"

class Line
{
public:
	Line(std::string text);
	Character& getCharacter(int charNum);
	void insert(char character, int charNum);
	void append(std::string text);
	void erase(int charNum);
	bool errored();
	bool warned();
	void resetIssues();
	void setErrored(bool errored);
	void setWarned(bool warned);
	void setIssue(std::string issue);
	std::string getIssue() const;
	std::string& getText();
	~Line();

private:
	std::vector<Character> m_characters;
	std::string m_line;
	std::string m_issue;
	bool m_errored = false;
	bool m_warned = false;
};

