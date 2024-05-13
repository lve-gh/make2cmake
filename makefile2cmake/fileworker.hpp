#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "info.hpp"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::endl;

vector<string> MakefileReader(string str);

void CMakeListsCreater(Info info);