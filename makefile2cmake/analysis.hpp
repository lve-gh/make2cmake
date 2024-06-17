#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <set>
#include "target.hpp"
#include "info.hpp"
#include "workWithStrings.hpp"
#include <boost/algorithm/string.hpp>

enum TARGET_ANALYSIS { WITH_COMPILER = 0, WITH_COMMAND, ONLY_OBJECTS, ONLY_CPPS, MIXED, NO_INFO };


using std::string;
using std::vector;
using std::cout;
using std::endl;

string DefineCompiler(vector<string> makefile);

string DefineFlags(vector<string> makefile);

vector<std::string> ExtractFlags(std::string str, vector<string> makefile);

string TransformLinksInVars(string input, vector<string> makefile);

bool IsCPP(string &word);

bool IsHeader(string &word);

bool IsAssigment(string &word);

string RemoveDuplicateWords(string& input_string);

bool IsIFDEF(string &word);

bool IsENDIF(string& word);

bool IsInclude(string &word);

bool IsTarget(string &word);

bool IsWithCommand(string& word);

bool IsWithCompiler(string& word, string& compiler);

bool ReadAllFiles(vector<string>& files);

string IsContains(string &word, vector<string> &words);

string ReturnArgsFromString(string& str);

string ReturnCommandFromString(string& str);

string ReturnCPPFromString(string& str);

int LinksInfo(string& str);

vector<string> SplitString(string& str);