#include <iostream>
#include <sstream>
#include <algorithm>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"
using namespace std;
using namespace std::filesystem;

int StringAnalysis(string word);
void ConvertingString(string makefileWord, int line, vector<string> &makefileWords, vector<Info> &info, string compiler, vector<string>& targets, vector<string>& rulesArg);
void StringProcessing(string& str);
vector<string> GraphAnalysis(vector<pair<string, pair<vector<string>, vector<string>>>>& graph, string compiler);
void PrintGraph(vector<pair<string, pair<vector<string>, vector<string>>>>& graph);
string ObjectProcessing(string str);
int main()
{
    ifstream infoFile;
    infoFile.open("info", ios::in | ios::out);
    string MakefileName;
    infoFile >> MakefileName;
    string compiler;
    infoFile >> compiler;
    infoFile.close();

    ifstream makefile;
    vector<string> makefileWords = MakefileReader(MakefileName);
    for (int i = 0; i < makefileWords.size(); i++) {
        StringProcessing(makefileWords[i]);
    }

    vector<Info> info;

    vector<pair<string, pair<vector<string>, vector<string>>>> graph;

    vector<string> targets;
    vector<string> rules;
    for (int i = 0; i < makefileWords.size(); i++) {
        ConvertingString(makefileWords[i], i, makefileWords, info, compiler, targets, rules);
    }

    std::reverse(targets.begin(), targets.end());
    std::reverse(rules.begin(), rules.end());

    for (int i = 0; i < targets.size() - 1; i++) {
        switch (LinksInfo(rules[i])) {
        case ONLY_CPPS: {
            Info infoTemp;
            infoTemp.target = "add_library(" + targets[i] + " OBJECT " + rules[i] + ")";
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";
            info.push_back(infoTemp);
            break;
        }
        case ONLY_OBJECTS: {
            Info infoTemp;
            infoTemp.target = "add_library(" + targets[i] + " " + ObjectProcessing(rules[i]) + ")";
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";
            info.push_back(infoTemp);
            break;
        }
        case MIXED: {
            break;
        }
        default:
            break;
        }
    }

    Info infoTemp;
    infoTemp.target = "add_executable(" + targets[targets.size() - 1] + " " + rules[rules.size() - 1] + ")";
    infoTemp.token = TARGET;
    infoTemp.version = "3.0.2";
    info.push_back(infoTemp);

    //cout << targetsInfo.size();
    //for (int i = 0; i < targets.size(); i++) {
    //    Info infoTemp;
    //    infoTemp.target = targets[i];
    //    infoTemp.token = TARGET;
    //    infoTemp.version = "3.0.2";
    //    info.push_back(infoTemp);
    //}

    CMakeListsCreaterNew(info);
    info.clear();
    return 0;
}



 int StringAnalysis(string word) {
     if (IsCommand(word))
         return COMMAND;
     else if (IsInclude(word))
         return INCLUDE;
     else if (IsIFDEF(word))
         return IFDEF;
     else if (IsENDIF(word))
         return ENDIF;
     else if (IsTarget(word))
         return TARGET;
     else if (IsAssigment(word))
         return ASSIGMENT;
     else
         return UNDEFINED;
}

 int RulesAnalysis(string word, string compiler) {
     if (IsWithCompiler(word, compiler))
         return WITH_COMPILER;
     else if (IsWithCommand(word))
         return WITH_COMMAND;
     else
         return NO_INFO;
 }

void ConvertingString(string makefileWord, int line, vector<string> &makefileWords, vector<Info> &info, string compiler, vector<string>& targets, vector<string>& rulesArg) {
    switch (StringAnalysis(makefileWord)) {
    case COMMAND: {
        for(size_t i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        size_t pos = makefileWord.find("=");
        if (pos != string::npos)
        {
            Info infoTemp;
            infoTemp.command = make_pair (makefileWord.substr(0, pos), makefileWord.substr(pos + 1));
            infoTemp.token = COMMAND;
            infoTemp.version = "3.0.2";
            info.push_back(infoTemp);
        }
        break;
    }
    case INCLUDE: {
        for (size_t i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        size_t pos = makefileWord.find("include");
        string nameString = makefileWord.substr(pos + 7);
        vector<string> names;

        stringstream ss(nameString);
        string name;
        while (getline(ss, name, ' ')) {
            names.push_back(name);
        }

        for (int i = 0; i < names.size(); i++) {
            vector<string> makefileWordsTemp = MakefileReader(names[i]);
            for(int j = 0; j < makefileWordsTemp.size(); j++)
                ConvertingString(makefileWordsTemp[j], j, makefileWords, info, compiler, targets, rulesArg); 
        }
        break;
    }
    case IFDEF: {
        size_t pos = makefileWord.find("ifdef");
        if (pos != string::npos)
        {
            Info infoTemp;
            infoTemp.ifdefVar = makefileWord.substr(pos + 6);
            infoTemp.token = IFDEF;
            infoTemp.version = "3.0.2";
            info.emplace_back(infoTemp);
        }
        break;
    }
    case ENDIF: {
        Info infoTemp;
        infoTemp.token = ENDIF;
        infoTemp.version = "3.0.2";
        info.emplace_back(infoTemp);
        break;
    }
    case TARGET: {
        size_t pos = makefileWord.find(":");
        if (pos != string::npos)
        {
            string rulesTemp = makefileWord.substr(0, pos);
            vector<string> rules;
            stringstream ss(makefileWord.substr(pos + 1));
            while (getline(ss, rulesTemp, ' ')) {
                rules.push_back(rulesTemp);
            }

            string target = makefileWord.substr(0, pos);
            string rule = makefileWord.substr(pos + 1);

            vector<string> recipes;
            for (size_t i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) != TARGET && StringAnalysis(makefileWords[line + i]) != ASSIGMENT; i++)
                recipes.push_back(makefileWords[line + i]);

            Info infoTemp;
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";

            bool withCommands = false;
            for (int j = 0; j < recipes.size(); j++) {
                if (IsWithCommand(recipes[j])) {
                    withCommands = true;
                    if (!IsWithCompiler(recipes[j], compiler)) {

                        infoTemp.target = "add_custom_command(OUTPUT " + ReturnCPPFromString(recipes[j]) + " COMMAND " + ReturnCommandFromString(recipes[j]) + ")";
                    }
                    else {
                        infoTemp.target = "add_custom_target(command COMMAND " + ReturnCommandFromString(rules[j]) + ")";
                    }
                }
            }

            if (withCommands == false) {
                targets.emplace_back(target);
                rulesArg.emplace_back(rule);
            }
            withCommands = false;
            rules.clear();
            recipes.clear();
        }
        break;
    }
    /*
    case TARGET: {
        size_t pos = makefileWord.find(":");
        if (pos != string::npos)
        {
            string rulesTemp = makefileWord.substr(0, pos);
            vector<string> rules;
            stringstream ss(makefileWord.substr(pos + 1));
            while (getline(ss, rulesTemp, ' ')) {
                rules.push_back(rulesTemp);
            }

            string target = makefileWord.substr(0, pos);

            vector<string> recipes;
            for (size_t i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) != TARGET && StringAnalysis(makefileWords[line + i]) != ASSIGMENT; i++)
                recipes.push_back(makefileWords[line + i]);


            Info infoTemp;
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";

            string allRules;
            bool withCommands = false;
                for (int j = 0; j < recipes.size(); j++) {
                    if (IsWithCommand(recipes[j])) {
                        withCommands = true;
                        if (!IsWithCompiler(recipes[j], compiler)) {
                            
                            //infoTemp.target = "add_custom_command(OUTPUT " + ReturnCPPFromString(recipes[j]) + " COMMAND " + ReturnCommandFromString(recipes[j]) + ")";
                            //targets.emplace_back("add_custom_command(OUTPUT " + ReturnCPPFromString(recipes[j]) + " COMMAND " + ReturnCommandFromString(recipes[j]) + ")");
                        }
                        else {
                            //infoTemp.target = "add_custom_target(command COMMAND " + ReturnCommandFromString(rules[j]) + ")";
                            //targets.emplace_back("add_custom_target(command COMMAND " + ReturnCommandFromString(rules[j]) + ")");
                            //targets.emplace_back("$<TARGET_OBJECTS:" + ReturnCommandFromString(rules[j]) + "> ");
                        }
                    }
                }
                if (withCommands == false) {
                    for (int j = 0; j < rules.size(); j++) {
                        if (LinksInfo(rules[j]) == ONLY_OBJECTS) {
                            //targets.emplace_back("target_link_libraries(" + target + " " + rules[j] + ")");
                            allRules += rules[j];
                            //targets.emplace_back("$<TARGET_OBJECTS:" + rules[j] + "> ");
                        }
                        if (LinksInfo(rules[j]) == ONLY_OBJECTS)
                            rules.emplace_back(allRules);
                        if (LinksInfo(rules[j]) == ONLY_CPPS) {
                            //infoTemp.target = "add_library(" + target + " " + rules[j] + ")";
                            allRules += rules[j];
                            //targets.emplace_back("add_library(" + target + " " + rules[j] + ")");
                        }
                        if (LinksInfo(rules[j]) == ONLY_CPPS)
                            rules.emplace_back(allRules);
                        if (LinksInfo(rules[j]) == MIXED) {
                            //infoTemp.target = "target_link_libraries(" + target + " " + rules[j] + ")";
                            targets.emplace_back("target_link_libraries(" + target + " " + rules[j] + ")");
                        }
                    }
                }
                withCommands = false;
                rules.clear();
                recipes.clear();
        }
        break;
    }
    */
    case ASSIGMENT: {
        for (size_t i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        size_t pos = makefileWord.find("=");
        if (pos != string::npos)
        {
            Info infoTemp;
            infoTemp.assigment = make_pair(makefileWord.substr(0, pos), (makefileWord.substr(pos + 1)));
            infoTemp.token = ASSIGMENT;
            infoTemp.version = "3.0.2";
            info.emplace_back(infoTemp);
        }
        break;
    }
    case UNDEFINED: {
        break;
    }
    default:
        break;
    }
}

void StringProcessing(string& str) {
    size_t posComment = str.find("#");
    if (posComment != string::npos)
    {
        str = str.substr(0, posComment);
    }

    std::string result(str);

    size_t posLink = result.find("$(");
    while (posLink != std::string::npos) {
        result.replace(posLink, 2, "${");
        posLink = result.find("$(", posLink);
    }
    str = result;

    posLink = result.find(")");
    while (posLink != std::string::npos) {
        result.replace(posLink, sizeof(")"), "} ");
        posLink = result.find(")", posLink);
    }
    str = result;
}

vector<string> GraphAnalysis(vector<pair<string, pair<vector<string>, vector<string>>>>& graph, string compiler) {
    vector<string> targetsInfo;
    string info;
    bool withCommands = false;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].second.second.size(); j++) {
            if (IsWithCommand(graph[i].second.second[j])) {
                withCommands = true;
                if (!IsWithCompiler(graph[i].second.second[j], compiler)) {
                    info = "add_custom_command(OUTPUT " + ReturnCPPFromString(graph[i].second.second[j]) + " COMMAND " + ReturnCommandFromString(graph[i].second.second[j]) + ")";
                    targetsInfo.push_back(info);
                }
                else {
                    info = "add_custom_target(command COMMAND " + ReturnCommandFromString(graph[i].second.second[j]) + ")";
                    targetsInfo.push_back(info);
                }
            }
        }
        if (withCommands == false) {
            for (int j = 0; j < graph[i].second.first.size(); j++) {
                if (LinksInfo(graph[i].second.first[j]) == ONLY_OBJECTS) {
                    info = "target_link_libraries(" + graph[i].first + " " + graph[i].second.first[j] + ")";
                    targetsInfo.push_back(info);
                }
                if (LinksInfo(graph[i].second.first[j]) == ONLY_CPPS) {
                    info = "add_executable(" + graph[i].first + " " + graph[i].second.first[j] + ")";
                    targetsInfo.push_back(info);
                }
                if (LinksInfo(graph[i].second.first[j]) == MIXED) {
                    info = "target_link_libraries(" + graph[i].first + " " + graph[i].second.first[j] + ")";
                    targetsInfo.push_back(info);
                }
            }
        }
        withCommands = false;
    }
    return targetsInfo;
}


void PrintGraph(vector<pair<string, pair<vector<string>, vector<string>>>>& graph) {
    for (int i = 0; i < graph.size(); i++) {
        cout << graph[i].first << " : ";
        for (int j = 0; j < graph[i].second.first.size(); j++) {
            cout << graph[i].second.first[j] << " ";
        }
        for (int j = 0; j < graph[i].second.second.size(); j++) {
            cout << graph[i].second.second[j] << " ";
        }
        cout << endl;
    }
}

string ObjectProcessing(string str) {
    vector<string> words;
    string current_word;
    for (char ch : str) {
        if (ch == ' ') {
            if (!current_word.empty()) {
                words.push_back(current_word);
                current_word.clear();
            }
        }
        else {
            current_word += ch;
        }
    }
    if (!current_word.empty()) {
        words.push_back(current_word);
    }
    for (int i = 0; i < words.size(); i++) {
        string temp = words[i];
        words[i] = "$<TARGET_OBJECTS:" + temp + ">";
    }
    str = "";
    for (int i = 0; i < words.size(); i++) {
        str += words[i] + " ";
    }
    return str;
}