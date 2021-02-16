#pragma once

#include <string>
#include <vector>

/*
 *	All message prompts are string vectors.
 */

inline
std::vector<std::string> vstrResizeMsg{

	"Terminal size is too small to display the cell cluster.",
	"Please resize the terminal or make a smaller dish."
};

inline
std::vector<std::string> vstrHelp{

	"Conways' Game Of Life",
	"",
	"<F1>\t\t\t- Display help",
	"<F5>\t\t\t- Show/Hide grid",
	"",
	"The following commands are mode dependent:",
	"",
	"Running/Pause:",
	"\t<Spacebar>\t- Pause/Step",
	"",
	"Pattern:",
	"\t<Esc>\t\t- Menu",
	"\t<Arrow keys>\t- Move cursor",
	"\t<Spacebar>\t- Toggle cell state",
	"\t<L-Click>\t- Revive",
	"\t<Ctrl+L-Click>\t- Erase"
};

inline
std::vector<std::string> vstrNoNameGiven{

	"No name specified.",
	"File not saved."
};

inline
std::vector<std::string> vstrUnableToLoad{

	"Terminal size is too small to load this file.",
	"Please resize your terminal and try again."
};
