#include <iostream>
#include <sstream>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"
using namespace std;
using namespace std::filesystem;

enum CONTENT { NOTHING = 0, TARGET, CPP, HEADER, EQUALS_SIGN};

int WordAnalysis(string word);

int main(int argc, char* argv[])
{
    ifstream makefile;
    vector<string> makefileWords = MakefileReader("makefile");
    vector<Target> targets;
    Target target;
    vector<string> varName;
    vector<string> varValue;
    Info info;
    for (int i = 0; i < makefileWords.size(); i++) {
            switch (WordAnalysis(makefileWords[i])) {
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
                case CPP: {
                    if (!target.isExistInCPP(makefileWords[i])) 
                        target.AddToCPP(makefileWords[i]);
                    cout << endl;
                    break;
                }
                case HEADER: {
                    if (!target.isExistInHeader(makefileWords[i]))
                        target.AddToHeader(makefileWords[i]);
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
}

 int WordAnalysis(string word) {
     if (IsTarget(word))
         return TARGET;
     else if (IsHeader(word))
         return HEADER;
     else if (IsEqualsSign(word))
         return EQUALS_SIGN;
     else if (IsCPP(word))
         return CPP;
     else
         return NOTHING;
}
