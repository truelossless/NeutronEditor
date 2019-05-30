#include "InputHandler.h"

#include <nfd.h>
#include <iostream>

#include "SyntaxHighlighter.h"

InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

InputHandler::Result InputHandler::handleTextView(sf::Event& event, TextView& textView) {

	Result res;

	Cursor* cursor = textView.getCursor();
	int currentLineLength = textView.getLine(cursor->getLine()).getText().length();

	bool ctrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
	bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
	bool altPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);

	if (event.type == sf::Event::KeyPressed) {

		if (event.key.code == sf::Keyboard::Left) {
			if (cursor->getPos() > 0) {

				// jump to the last word
				if (ctrlPressed) {
					int previousWordPos = textView.getLine(cursor->getLine()).getText().rfind(' ', cursor->getPos() - 2);
					if (previousWordPos != std::string::npos) {
						previousWordPos++;
						while (textView.getLine(cursor->getLine()).getText()[previousWordPos] == ' ') {
							previousWordPos--;
						}
						if (previousWordPos < 0) previousWordPos = 0;
					} else {
						previousWordPos = 0;
					}

					cursor->setPos(previousWordPos);
				}
				else {
					cursor->left();
				}
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
			}
			else if (cursor->getLine() > 0) {
				cursor->up();
				cursor->setPos(textView.getLine(cursor->getLine()).getText().length());
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine() + 1));
			}
		}

		else if (event.key.code == sf::Keyboard::Right) {

			if (cursor->getPos() < currentLineLength) {

				// jump to the next word
				if (ctrlPressed) {
					int nextWordPos = textView.getLine(cursor->getLine()).getText().find(' ', cursor->getPos());
					if (nextWordPos != std::string::npos) {
						nextWordPos++;
						while (textView.getLine(cursor->getLine()).getText()[nextWordPos] == ' ') {
							nextWordPos++;
						}
					}
					else {
						nextWordPos = textView.getLine(cursor->getLine()).getText().length();
					}

					cursor->setPos(nextWordPos);
				}
				else {
					cursor->right();
				}
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
			}
			else if (cursor->getLine() < textView.getLineCount() - 1) {
				cursor->setPos(0);
				cursor->down();
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine() - 1));
			}
		}

		else if (event.key.code == sf::Keyboard::Up) {
			if (cursor->getLine() > 0) {

				cursor->up();

				if (cursor->getPos() > textView.getLine(cursor->getLine()).getText().length()) {
					cursor->setPos(textView.getLine(cursor->getLine()).getText().length());
				}

				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine() + 1));
			}
		}

		else if (event.key.code == sf::Keyboard::Down) {
			if (cursor->getLine() < textView.getLineCount() - 1) {
				cursor->down();


				if (cursor->getPos() > textView.getLine(cursor->getLine()).getText().length()) {
					cursor->setPos(textView.getLine(cursor->getLine()).getText().length());
				}

				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine() - 1));

			}
		}

		else if (event.key.code == sf::Keyboard::Delete) {

			// delete the whole line
			if (shiftPressed) {

				if (cursor->getLine() == 0) {
					textView.addLine("");
				}

				textView.deleteLine(cursor->getLine());

				if (cursor->getLine() == textView.getLineCount()) {
					cursor->up();
				}

				if (cursor->getPos() > textView.getLine(cursor->getLine()).getText().length()) {
					cursor->setPos(textView.getLine(cursor->getLine()).getText().length());
				}

				SyntaxHighlighter::highlight(textView.getLine(cursor->getLine()));
			}
			else {
				if (cursor->getPos() < currentLineLength) {
					textView.erase(cursor->getPos(), cursor->getLine());
				}
				else if (textView.getLineCount() - 1 > cursor->getLine()) {
					textView.backspaceDeleteLine(cursor->getLine() + 1);
				}
			}

		}

		// paste text
		else if (event.key.code == sf::Keyboard::V && ctrlPressed) {
		std::string clipboardContent = sf::Clipboard::getString();
			textView.paste(clipboardContent);
		}

		// save a file
		else if (event.key.code == sf::Keyboard::S && ctrlPressed) {
			textView.save();
		}
	}

	else if (event.type == sf::Event::TextEntered) {

		if (event.text.unicode == '\b') {
			// backspace

			int eraseNum = 0;

			// delete all spaces together
			for (int i = cursor->getPos() - 1; i >= 0; i--) {
				if (textView.getLine(cursor->getLine()).getText()[i] == ' ') eraseNum++;
				else break;
			}

			if (eraseNum < 1) eraseNum = 1;

			// basic erase
			if (cursor->getPos() > 0) {
				for (int i = 0; i < eraseNum; i++) {
					textView.erase(cursor->getPos() - 1, cursor->getLine());
					cursor->left();
				}
			}

			// append line to the previous one
			else if (cursor->getLine() > 0) {
				cursor->setPos(textView.backspaceDeleteLine(cursor->getLine()));
				cursor->up();
			}
		}

		// enter
		else if (event.text.unicode == '\r') {

			cursor->setPos(textView.enterInsertLine(cursor->getPos(), cursor->getLine()));
			cursor->down();
		}

		// tab
		else if (event.text.unicode == '\t') {

			for (int i = 0; i < 4; i++) {
				textView.insert(' ', cursor->getPos() + i, cursor->getLine());
			}
			cursor->right(4);
		}

		else if (event.text.unicode == '{') {
			textView.insert('{', cursor->getPos(), cursor->getLine());
			cursor->right();
			textView.insert('}', cursor->getPos(), cursor->getLine());
		}

		else if (event.text.unicode == '(') {
			textView.insert('(', cursor->getPos(), cursor->getLine());
			cursor->right();
			textView.insert(')', cursor->getPos(), cursor->getLine());
		}

		else if (event.text.unicode == '[') {
			textView.insert('[', cursor->getPos(), cursor->getLine());
			cursor->right();
			textView.insert(']', cursor->getPos(), cursor->getLine());
		}

		else if (event.text.unicode == '"') {
			textView.insert('"', cursor->getPos(), cursor->getLine());
			cursor->right();
			textView.insert('"', cursor->getPos(), cursor->getLine());
		}

		else if (event.text.unicode == '\'') {
			textView.insert('\'', cursor->getPos(), cursor->getLine());
			cursor->right();
			textView.insert('\'', cursor->getPos(), cursor->getLine());
		}

		// actionbar shortcut
		else if (event.text.unicode == ' ' && shiftPressed) {
			res.focusChanged = true;
			res.newFocus = Focus::ActionBar;
		}

		// escape key
		else if (event.text.unicode == 27) {

		}

		// CTRL + ALT is ALT GR
		else if (!ctrlPressed || ctrlPressed && altPressed) {
			textView.insert(static_cast<char>(event.text.unicode), cursor->getPos(), cursor->getLine());
			cursor->right();
		}
	}

	return res;
}

InputHandler::Result InputHandler::handleActionBar(sf::Event& event, ActionBar& actionBar) {

	Result res;

	if (event.type == sf::Event::KeyPressed) {

		if (event.key.code == sf::Keyboard::Escape) {
			res.focusChanged = true;
			res.newFocus = Focus::TextView;
		}
	}
	else if (event.type == sf::Event::TextEntered) {

		if (event.text.unicode == '\r') {
			actionBar.submit();
		}

		// backspace
		else if (event.text.unicode == '\b') {
			if (!actionBar.getText().empty()) actionBar.erase();
		}
		else {
			actionBar.insert(event.text.unicode);
		}
	}

	return res;
}