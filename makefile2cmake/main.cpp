#include <iostream>
#include <sstream>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"
using namespace std;
using namespace std::filesystem;

enum CONTENT { NOTHING = 0, COMMAND, TARGET, EQUALS_SIGN};

int WordAnalysis(string word);

int main(int argc, char* argv[])
{
    ifstream makefile;
    vector<string> makefileWords = MakefileReader("Makefile");
    vector<Target> targets;
    Target target;
    Info info;
    for (int i = 0; i < makefileWords.size(); i++) {
            switch (WordAnalysis(makefileWords[i])) {
                case COMMAND: {
                    int pos = makefileWords[i].find("=");
                    if (pos != string::npos)
                    {
                        info.commandName.emplace_back(makefileWords[i].substr(0, pos));
                        info.commandValue.emplace_back(makefileWords[i].substr(pos + 1));
                    }
                    break;
                }
                case TARGET: {
                    int pos = makefileWords[i].find(":");
                    if (pos != string::npos)
                    {
                        Target targetTemp;
                        targetTemp.SetName(makefileWords[i].substr(0, pos));
                        targetTemp.SetLinks(makefileWords[i].substr(pos + 1));
                        info.targets.emplace_back(targetTemp);
                    }
                    break;
                }
                case EQUALS_SIGN: {
                    int pos = makefileWords[i].find("=");
                    if (pos != string::npos)
                    {
                        info.varName.emplace_back(makefileWords[i].substr(0, pos));
                        info.varValue.emplace_back(makefileWords[i].substr(pos + 1));
                    }
                    break;
                }
                default:
                    break;
            }
    }
    targets.emplace_back(target);
    CMakeListsCreater(info);
    return 0;
}

 int WordAnalysis(string word) {
     if (IsCommand(word))
         return COMMAND;
     else if (IsTarget(word))
         return TARGET;
     else if (IsEqualsSign(word))
         return EQUALS_SIGN;
     else
         return NOTHING;
}
