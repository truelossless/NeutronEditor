#include "SyntaxHighlighter.h"

#include <iostream>
#include <map>

#include "Constants.h"

const char SyntaxHighlighter::m_separators[] = {
	' ',
	'(',
	')',
	'{',
	'}',
	';',
	',',
	':',
	'[',
	']',
	'|',
	'<',
	'>',
	'=',
	'&',
	'!'
};

const std::string SyntaxHighlighter::m_operators[] = {
	
	"for",
	"while",
	"if",
	"else",
	"do",
	"switch",
	"case",
	"return"
};

const std::string SyntaxHighlighter::m_types[] = {

	"void",

	"class",

	"boolean",
	"int",
	"long",
	"double",
	"float",
	"size_t",
	"char",
	
	"unsigned",
	"const",

	"static",
	"private",
	"protected",
	"public"
};

const std::string SyntaxHighlighter::m_numbers[] = {

	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9"
};

const char SyntaxHighlighter::m_quotes[] = {
	'"',
	'\'',
	'`'
};

void SyntaxHighlighter::highlight(Line & line) {

	std::map<int, bool> insideQuotes;

	// remove all the colors
	for (int i = 0; i < line.getText().length(); i++) {
		line.getCharacter(i).getText().setFillColor(Constants::TEXT_COLOR);
	}

	// highlight numbers
	checkArray(m_numbers, m_numbers_size, line, Constants::NUMBER_COLOR, m_numbers, m_numbers_size);

	// highlight important keywords
	checkArray(m_operators, m_operators_size, line, Constants::OPERATOR_COLOR);
	checkArray(m_types, m_types_size, line, Constants::TYPE_COLOR);

	// highlight text inside quotes
	for (int i = 0; i < m_quotes_size; i++) {

		int pos = static_cast<int>(line.getText().find(m_quotes[i], 0));
		if (pos != std::string::npos && !insideQuotes[pos]) {

			// color the first quote
			line.getCharacter(pos).getText().setFillColor(Constants::QUOTES_COLOR);
			
			pos++;
			while (pos < line.getText().length() && line.getText()[pos] != m_quotes[i]) {
				
				// color the text inside
				line.getCharacter(pos).getText().setFillColor(Constants::QUOTES_COLOR);
				insideQuotes[pos] = true;
				pos++;
			}

			// color the second quote
			if (pos < line.getText().length()) {
				line.getCharacter(pos).getText().setFillColor(Constants::QUOTES_COLOR);
			}
		}
	}

	// highlight inline comments
	int pos = static_cast<int>(line.getText().find("//", 0));

	while (pos != std::string::npos) {

		if (!insideQuotes[pos]) {
			for (int i = pos; i < line.getText().length(); i++) {
				line.getCharacter(i).getText().setFillColor(Constants::COMMENT_COLOR);
			}
		}

		pos = static_cast<int>(line.getText().find("//", pos+2));
	}
}

void SyntaxHighlighter::highlightCharacter(Character& character, sf::Color& color) {
	character.getText().setFillColor(color);
}

void SyntaxHighlighter::checkArray(const std::string arr[], int size, Line& line, sf::Color& color, const std::string separators2[], int size2) {
	
	for (int i = 0; i < size; i++) {

		// get the position of the operator if it's in the line
		int pos = static_cast<int>(line.getText().find(arr[i], 0));

		while (pos != std::string::npos) {

			int nextCharPos = static_cast<int>(arr[i].length() + pos);
			int lastCharPos = pos - 1;

			// check if the word isn't in another word
			// we could write this as one big if but it's more readable that way
			bool checkForNextChar = true;
			bool checkForLastChar = true;
			
			int  checkPassed = 0;

			std::string lineText = line.getText();

			if (nextCharPos == lineText.length()) {
				checkForNextChar = false;
				checkPassed++;
			}

			if (lastCharPos == -1) {
				checkForLastChar = false;
				checkPassed++;
			}

			if (checkForNextChar) {

				int checks = checkPassed;

				for (int i = 0; i < m_separators_size; i++) {
					if (m_separators[i] == lineText[nextCharPos]) {
						checkPassed++;
						break;
					}
				}
				
				// check the additionnal array given
				if (checks == checkPassed && size2 != 0) {
					for (int i = 0; i < size2; i++) {
						if (separators2[i][0] == lineText[nextCharPos]) {
							checkPassed++;
							break;
						}
					}
				}

			}

			if (checkForLastChar) {
				
				int checks = checkPassed;
				
				for (int i = 0; i < m_separators_size; i++) {
					if (m_separators[i] == lineText[lastCharPos]) {
						checkPassed++;
						break;
					}
				}

				// check the additionnal array given
				if (checks == checkPassed && size2 != 0) {
					for (int i = 0; i < size2; i++) {
						if (separators2[i][0] == lineText[lastCharPos]) {
							checkPassed++;
							break;
						}
					}
				}
			}
			
			// color the word if both checks passed
			if (checkPassed == 2) {
				// change the color of the j characters involved (the operator's length)
				for (int j = 0; j < arr[i].length(); j++) {
					line.getCharacter(pos + j).getText().setFillColor(color);
				}
			}

			pos = static_cast<int>(line.getText().find(arr[i], pos + 1));
		}
	}
}
