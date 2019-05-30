#include <json/json.h>
#include "ActionBar.h"

void ActionBar::action_setFontSize(ActionBar& actionBar) {
	
	actionBar.setRequest("New font size (in px): ", action_setFontSizeCallback);
}

void ActionBar::action_setFontSizeCallback(std::string text) {

	Json::Value json;
	std::fstream in("conf.json");
	
	in >> json;

	Json::Value newFontSize = std::stoi(text);
	json["editor"]["fontSize"].swapPayload(newFontSize);

	in.close();

	std::ofstream out("conf.json", std::ios::trunc);

	out << json;

	out.close();
}
