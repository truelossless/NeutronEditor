#include "ActionBar.h"

const std::vector<ActionBar::Command> ActionBar::m_commands = {

	{
		"save and close the current file",
		{'f', 'c'},
		ActionBar::action_close
	},

	{
		"close the current file without saving",
		{'f', 'C'},
		ActionBar::action_forceClose
	},

	{
		"open a file",
		{'f', 'o'},
		ActionBar::action_open
	},

	{
		"save the current file",
		{'f', 's'},
		ActionBar::action_save
	},

	{
		"open the json configuration file",
		{'s', 'c', 'o'},
		ActionBar::action_openConfFile
	},

	{
		"set the editor's font size",
		{'s', 'e', 'f'},
		ActionBar::action_setFontSize
	},

	{
		"open the json theme file",
		{'s', 't', 'o'},
		ActionBar::action_openThemeFile
	},

	{
		"reload the theme",
		{'s', 't', 'r'},
		ActionBar::action_reloadTheme
	},

	{
		"merge window horizontally",
		{'w', 'm', 'h'},
		ActionBar::action_mergeHorizontally
	},

	{
		"merge window vertically",
		{'w', 'm', 'v'},
		ActionBar::action_mergeVertically
	},
	{
		"split window horizontally",
		{'w', 's', 'h'},
		ActionBar::action_splitHorizontally
	},

	{
		"split window vertically",
		{'w', 's', 'v'},
		ActionBar::action_splitVertically
	},


};