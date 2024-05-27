#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <iostream>

enum TARGET_ANALYSIS { WITH_COMPILER = 0, WITH_COMMAND, ONLY_OBJECTS, ONLY_CPPS, MIXED, NO_INFO };


using std::string;
using std::vector;
using namespace std::filesystem;

bool IsCPP(string &word);

bool IsHeader(string &word);

bool IsAssigment(string &word);

bool IsIFDEF(string &word);

bool IsENDIF(string& word);

bool IsInclude(string &word);

bool IsCommand(string &word);

//bool IsRecipe(string& word);

bool IsTarget(string &word);

bool IsWithCommand(string& word);

bool IsWithCompiler(string& word, string& compiler);

bool ReadAllFiles(vector<string>& files);

string IsContains(string &word, vector<string> &words);

string ReturnCommandFromString(string& str);

string ReturnCPPFromString(string& str);

int LinksInfo(string& str);

vector<string> SplitString(string& str);