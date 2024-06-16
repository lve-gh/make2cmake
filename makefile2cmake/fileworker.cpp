#include "fileworker.hpp"
#include <iostream>
vector<string> MakefileReader(string str) {
    ifstream makefile;
    vector<string> makefileWords;
    makefile.open(str, ios::in | ios::out);
    string word = "";
    while (std::getline(makefile, word)) {
        makefileWords.emplace_back(word);
    }
    makefile.close();
    return makefileWords;
}

void CMakeListsCreaterNew(vector<Info>& info, string compiler, string flags) {
    ofstream cmakelists;
    cmakelists.open("CMakeLists.txt");
    cmakelists << "cmake_minimum_required(VERSION 3.29)" << endl;
    string projectName = "projectName";
    cmakelists << "project(" << projectName << ")" << endl;
    cmakelists << "set(CMAKE_C_COMPILER " + compiler + ")" << endl;
    cmakelists << "set(CMAKE_CPP_COMPILER " + compiler + ")" << endl;
    for (size_t i = 0; i < info.size(); i++) {
        cmakelists << info[i].WriteCMake() << endl;
    }

}