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

    for (int i = 0; i < info.targets.size(); i++) {
        if(info.targets[i].GetLinks().find(".cpp") != std::string::npos || 
           info.targets[i].GetLinks().find(".c") != std::string::npos || 
           info.targets[i].GetLinks().find(".h") != std::string::npos || 
           info.targets[i].GetLinks().find(".hpp") != std::string::npos)
            cmakelists << "add_library(" << info.targets[i].GetName() << " " << info.targets[i].GetLinks() << ")" << endl;
        else
            cmakelists << "target_link_libraries(" << info.targets[i].GetName() << " " << info.targets[i].GetLinks() << ")" << endl;
    }

    for (int i = 0; i < info.commandName.size(); i++) {
        cmakelists << "set(" << info.commandName[i] << ")" << endl;
        cmakelists << "execute_progress(COMMAND " << info.commandValue[i] << " WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE $(" << info.commandName[i] << "))" << endl;
    }

    cmakelists.close();
}