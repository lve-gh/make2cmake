#pragma once
#include <vector>
#include "target.hpp"

using std::vector;

class Info {
public:
//private: 
	vector<Target> targets;
	vector<string> varName;
	vector<string> varValue;
	vector<string> commandName;
	vector<string> commandValue;
//public:
};