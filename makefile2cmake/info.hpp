#pragma once
#include <vector>
#include <map>
#include "target.hpp"

enum CONTENT { UNDEFINED = 0, INCLUDE, IFDEF, ENDIF ,COMMAND, RECIPE, TARGET, ASSIGMENT };

using std::vector;
using std::pair;

class Info {
public:
	pair<string, string> assigment;
	string target;
	pair<string, string> command;
	string ifdefVar = "";
	int token = UNDEFINED;
	string version = "3.29";
	string WriteCMake();
};