#include <iostream>
#include <sstream>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"
using namespace std;
using namespace std::filesystem;

//enum TOKENS { UNDEFINED = 0, INCLUDE, IFDEF, COMMAND, TARGET, ASSIGMENT};
//enum TARGET_ANALYSIS { WITH_COMPILER = 0, WITH_COMMAND, NO_INFO};

int StringAnalysis(string word);
void ConvertingString(string makefileWord, int line, vector<string> &makefileWords, vector<Info> &info, vector<pair<string, pair<vector<string>, vector<string>>>> &graph, string compiler);
void StringProcessing(string& str);
vector<string> GraphAnalysis(vector<pair<string, pair<vector<string>, vector<string>>>>& graph, string compiler);
void PrintGraph(vector<pair<string, pair<vector<string>, vector<string>>>>& graph);
int main(int argc, char* argv[])
{
    ifstream makefile;
    //vector<string> makefileWords = MakefileReader(argv[0]);
    vector<string> makefileWords = MakefileReader("Makefile");
    for (int i = 0; i < makefileWords.size(); i++) {
        StringProcessing(makefileWords[i]);
    }

    vector<Info> info;

    vector<pair<string, pair<vector<string>, vector<string>>>> graph;

    for (int i = 0; i < makefileWords.size(); i++) {
        ConvertingString(makefileWords[i], i, makefileWords, info, graph, "gcc");
    }

    PrintGraph(graph);

    vector<string> targetsInfo = GraphAnalysis(graph, "gcc");
    for (int i = 0; i < targetsInfo.size(); i++) {
        Info infoTemp;
        infoTemp.target = targetsInfo[i];
        infoTemp.token = TARGET;
        infoTemp.version = "3.0.2";
        info.push_back(infoTemp);
    }

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

void ConvertingString(string makefileWord, int line, vector<string> &makefileWords, vector<Info> &info, vector<pair<string, pair<vector<string>, vector<string>>>> &graph, string compiler) {
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
        //cout << makefileWord;
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
                ConvertingString(makefileWordsTemp[j], j, makefileWords, info, graph, "gcc"); // arg!!!!
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
    /*
    case TARGET: {
        size_t pos = makefileWord.find(":");
        if (pos != string::npos)
        {
            Target targetTemp;
            targetTemp.SetName(makefileWord.substr(0, pos));
            targetTemp.SetLinks(makefileWord.substr(pos + 1));
            Info infoTemp;
            infoTemp.target = targetTemp;
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";
            info.emplace_back(infoTemp);

            ///////////////
            string namesTemp = makefileWord.substr(0, pos);
            vector<string> names;
            stringstream ss(makefileWord.substr(pos + 1));
            while (getline(ss, namesTemp, ' ')) {
                names.push_back(namesTemp);
            }

            string name = makefileWord.substr(0, pos);

            pair<string, vector<string>> graphElement;
            graphElement = make_pair (name, names);
            graph.push_back(graphElement);
            //graphElement
            //////////////////
        }
        break;
    }
    */

    /*
    case TARGET: {
        //cout << "1";
        size_t pos = makefileWord.find(":");
        if (pos != string::npos)
        {
            ///////////////
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

            pair<vector<string>, vector<string>> rulesAndRecipes;

            rulesAndRecipes = make_pair(rules, recipes);

            pair<string, pair<vector<string>, vector<string>>> graphElement;
            graphElement = make_pair(target, rulesAndRecipes);
            graph.push_back(graphElement);
            //graphElement
            //////////////////
        }
        break;
    } */

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

            string compiler = "gcc";

            Info infoTemp;
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";

            bool withCommands = false;
                for (int j = 0; j < recipes.size(); j++) {
                    if (IsWithCommand(recipes[j])) {
                        withCommands = true;
                        if (!IsWithCompiler(recipes[j], compiler)) {
                            infoTemp.target = "add_custom_command(OUTPUT " + ReturnCPPFromString(recipes[j]) + " COMMAND " + ReturnCommandFromString(recipes[j]) + ")";
                            info.emplace_back(infoTemp);
                        }
                        else {
                            infoTemp.target = "add_custom_target(command COMMAND " + ReturnCommandFromString(rules[j]) + ")";
                            info.emplace_back(infoTemp);
                        }
                    }
                }
                if (withCommands == false) {
                    for (int j = 0; j < rules.size(); j++) {
                        if (LinksInfo(rules[j]) == ONLY_OBJECTS) {
                            infoTemp.target = "target_link_libraries(" + target + " " + rules[j] + ")";
                            info.emplace_back(infoTemp);
                        }
                        if (LinksInfo(rules[j]) == ONLY_CPPS) {
                            infoTemp.target = "add_executable(" + target + " " + rules[j] + ")";
                            info.emplace_back(infoTemp);
                        }
                        if (LinksInfo(rules[j]) == MIXED) {
                            infoTemp.target = "target_link_libraries(" + target + " " + rules[j] + ")";
                            info.emplace_back(infoTemp);
                        }
                    }
                }
                withCommands = false;
                rules.clear();
                recipes.clear();
        }
        break;
    }
    case ASSIGMENT: {
        //cout << makefileWord << endl;
        for (size_t i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        size_t pos = makefileWord.find("=");
        if (pos != string::npos)
        {
            Info infoTemp;
            infoTemp.assigment = make_pair(makefileWord.substr(0, pos), (makefileWord.substr(pos + 1)));
            infoTemp.token = ASSIGMENT;
            infoTemp.version = "3.0.2";
            //cout << infoTemp.assigment.first << " " << infoTemp.assigment.second << endl;
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
        //cout << i;
        //cout << graph.size();
        for (int j = 0; j < graph[i].second.second.size(); j++) {
            if (IsWithCommand(graph[i].second.second[j])) {
                //cout << 1;
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