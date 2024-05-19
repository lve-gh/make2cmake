#include <iostream>
#include <sstream>
#include "target.hpp"
#include "analysis.hpp"
#include "fileworker.hpp"
#include "info.hpp"
using namespace std;
using namespace std::filesystem;

//endif
//enum TOKENS { UNDEFINED = 0, INCLUDE, IFDEF, COMMAND, TARGET, ASSIGMENT};

int StringAnalysis(string word);
void ConvertingString(string makefileWord, int line, vector<string> &makefileWords, vector<Info> &info);

int main(int argc, char* argv[])
{
    ifstream makefile;
    vector<string> makefileWords = MakefileReader("Makefile");
    //cout << makefileWords
    //for (int i = 0; i < makefileWords.size(); i++) {
    //    cout << makefileWords[i] << endl;
    //}

    vector<Info> info;
    for (int i = 0; i < makefileWords.size(); i++) {
        ConvertingString(makefileWords[i], i, makefileWords, info);
    }

    CMakeListsCreaterNew(info);
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

void ConvertingString(string makefileWord, int line, vector<string> &makefileWords, vector<Info> &info) {
    switch (StringAnalysis(makefileWord)) {
    case COMMAND: {
        for(int i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        int pos = makefileWord.find("=");
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
        for (int i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        //cout << makefileWord;
        int pos = makefileWord.find("include");
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
                ConvertingString(makefileWordsTemp[j], j, makefileWords, info);
        }
        break;
    }
    case IFDEF: {
        int pos = makefileWord.find("ifdef");
        if (pos != string::npos)
        {
            Info infoTemp;
            infoTemp.ifdefVar = makefileWord.substr(pos + 1);
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
    }
    case TARGET: {
        int pos = makefileWord.find(":");
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
        }
        break;
    }
    case ASSIGMENT: {
        //cout << makefileWord << endl;
        for (int i = 1; line + i < makefileWords.size() && StringAnalysis(makefileWords[line + i]) == UNDEFINED; i++)
            makefileWord += " " + makefileWords[line + i];
        int pos = makefileWord.find("=");
        if (pos != string::npos)
        {
            Info infoTemp;
            infoTemp.assigment = make_pair (makefileWord.substr(0, pos), (makefileWord.substr(pos + 1)));
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
