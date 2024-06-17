#include <iostream>
#include <sstream>
#include <algorithm>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"
#include "workWithStrings.hpp"
using std::stringstream;

int main()
{
    string MakefileName = "Makefile";
    ifstream makefile;
    vector<string> makefileWords = MakefileReader(MakefileName);
    for (size_t i = 0; i < makefileWords.size(); i++) {
        StringProcessing(makefileWords[i]);
    }

    vector<Info> info;

    vector<pair<string, pair<vector<string>, vector<string>>>> graph;

    vector<string> targets;
    vector<string> rules;
    vector<string> recipes;

    string findedCompiler = DefineCompiler(makefileWords);
    string findedFlafs = DefineFlags(makefileWords);

    for (size_t i = 0; i < makefileWords.size(); i++) {
        ConvertingString(makefileWords[i], i, makefileWords, info, findedCompiler, targets, rules, recipes);
    }

    std::reverse(targets.begin(), targets.end());
    std::reverse(rules.begin(), rules.end());
    std::reverse(recipes.begin(), recipes.end());

    for (size_t i = 0; i < targets.size() - 1 && targets.size() != 0; i++) {
        if (rules.size() >= i + 1) {
            switch (LinksInfo(rules[i])) {
            case ONLY_CPPS: {
                Info infoTemp;
                infoTemp.target = "add_library(" + targets[i] + " OBJECT " + rules[i] + ")";
                if(recipes[i] != "")
                    infoTemp.recipe = "target_compile_options(" + targets[i] + " PUBLIC " + RemoveDuplicateWords(recipes[i]) + ")";
                infoTemp.token = TARGET;
                infoTemp.version = "3.0.2";
                info.push_back(infoTemp);
                break;
            }
            case ONLY_OBJECTS: {
                Info infoTemp;
                infoTemp.target = "add_library(" + targets[i] + " " + ObjectProcessing(rules[i]) + ")";
                if (recipes[i] != "")
                    infoTemp.recipe = "target_compile_options(" + targets[i] + " PUBLIC " + RemoveDuplicateWords(recipes[i]) + ")";
                infoTemp.token = TARGET;
                infoTemp.version = "3.0.2";
                info.push_back(infoTemp);
                break;
            }
            case MIXED: {
                Info infoTemp;
                infoTemp.target = "add_library(" + targets[i] + " " + RuleChecking((rules[i])) + ")";
                if (recipes[i] != "")
                    infoTemp.recipe = "target_compile_options(" + targets[i] + " PUBLIC " + RemoveDuplicateWords(recipes[i]) + ")";
                infoTemp.token = TARGET;
                infoTemp.version = "3.0.2";
                info.push_back(infoTemp);
                break;
            }
            default:
                break;
            }
        }
    }


    Info infoTemp;
    if (targets.size() != 0 && rules.size()) {
        infoTemp.target = "add_executable(" + targets[targets.size() - 1] + " " + rules[rules.size() - 1] + ")";
        if(!IsCPP(rules[rules.size() - 1]) && !IsHeader(rules[rules.size() - 1]))
            infoTemp.target += "\ntarget_link_libraries(" + targets[targets.size() - 1] + " " + rules[rules.size() - 1] + ")";
        if (recipes[recipes.size() - 1] != "")
            infoTemp.recipe = "target_compile_options(" + targets[recipes.size() - 1] + " " + RemoveDuplicateWords(recipes[recipes.size() - 1]) + ")";
    }
        
    infoTemp.token = TARGET;
    infoTemp.version = "3.0.2";
    info.push_back(infoTemp);

    CMakeListsCreaterNew(info, findedCompiler, findedFlafs);
    info.clear();
    return 0;
}

