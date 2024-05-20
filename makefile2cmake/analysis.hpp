#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>

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

bool IsTarget(string &word);

bool ReadAllFiles(vector<string>& files);

string IsContains(string &word, vector<string> &words);