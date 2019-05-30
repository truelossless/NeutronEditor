#pragma once

#include <SFML/Graphics.hpp>

#include "TextView.h"
#include "ActionBar.h"

class InputHandler
{
public:

	
	enum class Focus {
		TextView,
		ActionBar
	};
	
	// a structure which holds the result of the inputs, and if we should keep the focus on that part of the window.
	struct Result {
		
		bool focusChanged = false;
		Focus newFocus;
		// id of the new focus
		int num = 0;
	};

	InputHandler();
	~InputHandler();
	
	Result handleTextView(sf::Event& event, TextView& textView);
	Result handleActionBar(sf::Event& event, ActionBar& actionBar);
};

