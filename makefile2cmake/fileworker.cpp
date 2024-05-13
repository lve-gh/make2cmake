#include "fileworker.hpp"

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

void CMakeListsCreater(Info info) {
    ofstream cmakelists;
    cmakelists.open("CMakeLists.txt");
    cmakelists << "cmake_minimum_required(VERSION 3.12)" << endl;
    string projectName = "projectName";
    cmakelists << "project(" << projectName << ")" << endl;

    for (int i = 0; i < info.varName.size(); i++)
        cmakelists << "set(" << info.varName[i] << " " << info.varValue[i] << ")" << endl;

    for (int i = 0; i < info.targets.size(); i++)
        cmakelists << "add_executable(" << info.targets[i].GetName() << " " << info.targets[i].GetLinks() << ")" << endl;

    cmakelists.close();
}