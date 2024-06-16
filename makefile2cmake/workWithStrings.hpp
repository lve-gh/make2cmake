#pragma once
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"

using std::vector;
using std::stringstream;
using std::string;

int StringAnalysis(string word);
void ConvertingString(string makefileWord, size_t line, vector<string>& makefileWords, vector<Info>& info, string compiler, vector<string>& targets, vector<string>& rulesArg, vector<string>& recipesArg);
void StringProcessing(string& str);
string ObjectProcessing(string str);
string RuleChecking(string str);