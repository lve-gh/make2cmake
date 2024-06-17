#include "workWithStrings.hpp"

int StringAnalysis(string word) {
    if (IsInclude(word))
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

void ConvertingString(string makefileWord, size_t line, vector<string>& makefileWords, vector<Info>& info, string compiler, vector<string>& targets, vector<string>& rulesArg, vector<string>& recipesArg) {
    switch (StringAnalysis(makefileWord)) {
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

        for (size_t i = 0; i < names.size(); i++) {
            vector<string> makefileWordsTemp = MakefileReader(names[i]);
            for (size_t j = 0; j < makefileWordsTemp.size(); j++)
                ConvertingString(makefileWordsTemp[j], j, makefileWords, info, compiler, targets, rulesArg, recipesArg);
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

            vector<vector<string>> flags;
            for (size_t i = 0; i < recipes.size(); i++)
                flags.push_back(ExtractFlags(recipes[i], makefileWords));

            string flagsStr = "";
            for (int i = 0; i < flags.size(); i++) {
                for (int j = 0; j < flags[i].size(); j++)
                    flagsStr += flags[i][j] + " ";
            }

            Info infoTemp;
            infoTemp.token = TARGET;
            infoTemp.version = "3.0.2";
            bool withCommands = false;
            for (size_t j = 0; j < recipes.size(); j++) {
                if (IsWithCommand(recipes[j])) {
                    withCommands = true;
                    if (!IsWithCompiler(recipes[j], compiler)) {
                        infoTemp.target = "add_custom_command(OUTPUT ..\\\\" + ReturnCPPFromString(recipes[j]) + " COMMAND " + ReturnCommandFromString(recipes[j]) + " ARGS ..\\\\" + ReturnArgsFromString(recipes[j]) + ")";
                        info.push_back(infoTemp);
                    }
                    else {
                        infoTemp.target = "add_custom_target(command COMMAND " + ReturnCommandFromString(rules[j]) + ")";
                        info.push_back(infoTemp);
                    }
                }
            }

            if (withCommands == false) {
                targets.emplace_back(target);
                rulesArg.emplace_back(rule);
                recipesArg.emplace_back(flagsStr);
            }

            rules.clear();
            recipes.clear();
        }
        break;
    }
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
    for (size_t i = 0; i < words.size(); i++) {
        string temp = words[i];
        words[i] = "$<TARGET_OBJECTS:" + temp + ">";
    }
    str = "";
    for (size_t i = 0; i < words.size(); i++) {
        str += words[i] + " ";
    }
    return str;
}

string RuleChecking(string str) {
    vector<string> words = SplitString(str);
    str = "";
    for (size_t i = 0; i < words.size(); i++) {
        if (words[i].find(".cpp") != std::string::npos || words[i].find(".hpp") != std::string::npos || words[i].find(".c") != std::string::npos || words[i].find(".h") != std::string::npos)
            str += " " + words[i] + " ";
        else
            str += " $<TARGET_OBJECTS:" + words[i] + ">";
    }
    return str;
}