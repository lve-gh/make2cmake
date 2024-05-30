#include "analysis.hpp"

using std::filesystem::recursive_directory_iterator;
using std::filesystem::begin;

bool IsCPP(string &word) {
    std::istringstream iss(word);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (token == "cpp" || token == "c") {
            return true;
        }
    }
    return false;
}

bool IsHeader(string &word) {
    std::istringstream iss(word);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (token == "hpp" || token == "h") {
            return true;
        }
    }
    return false;
}

bool IsAssigment(string &word) {
    if (word.find("=") != std::string::npos && !(word.find(":=") != std::string::npos))
        return true;
    return false;
}

bool IsIFDEF(string &word) {
    if (word.find("ifdef") != std::string::npos)
        return true;
    return false;
}

bool IsENDIF(string& word) {
    if (word.find("endif") != std::string::npos)
        return true;
    return false;
}

bool IsInclude(string &word) {
    if (word.find("include") != std::string::npos)
        return true;
    return false;
}

bool IsWithCommand(string& word) {
    std::istringstream iss(word);
    std::string token;
    if (word.find("./") != std::string::npos)
        return true;
    return false;
}

bool IsWithCompiler(string& word, string& compiler) {
    vector<string> words;
    std::stringstream ss(word);
    string wordInString;
    while (std::getline(ss, wordInString, ' ')) {
        words.push_back(wordInString);
    }
    for (int i = 0; i < words.size(); i++) {
        if (words[i] == compiler) {
            words.clear();
            return true;
        }
    }
    return false;
}


bool IsTarget(string &word) {
    std::istringstream iss(word);
    std::string token;
    if (word.find(":") != std::string::npos)
        return true;
    return false;
}

bool ReadAllFiles(vector<string>& files) {
    static recursive_directory_iterator di(".");
    static auto it = begin(di);
    if (it == end(di)) return false;
    files.emplace_back(it->path().generic_string());
    it++;
    return true;
}

string IsContains(string &word, vector<string> &words) {
    string wordTemp;
    for (int i = 0; i < words.size(); i++) {
        if (words[i].find(word) != string::npos) {
            wordTemp = words[i];
            words.clear();
            return wordTemp;
        }
    }
    words.clear();
    return "";
}

string ReturnCommandFromString(string& str) {
    vector<string> words = SplitString(str);
    string word;
    for (int i = 0; i < words.size(); i++) {
        if (words[i].find("./") != string::npos) {
            word = words[i];
            words.clear();
            return word;
        }
    }
    words.clear();
    return "";
}

string ReturnCPPFromString(string& str) {
    vector<string> words = SplitString(str);
    string word;
    for (int i = 0; i < words.size(); i++) {
        if (words[i].find(".hpp") != string::npos || words[i].find(".h") != string::npos || words[i].find(".cpp") != string::npos || words[i].find(".c") != string::npos) {
            word = words[i];
            words.clear();
            return word;
        }
    }
    return "";
}

int LinksInfo(string& str) {
    int objectsCount = 0;
    int cppsCount = 0;
    vector<string> words;
    std::stringstream ss(str);
    string wordInString;
    while (std::getline(ss, wordInString, ' ')) {
        words.push_back(wordInString);
    }
    for (int i = 0; i < words.size(); i++) {
        if (words[i].find(".hpp") != string::npos || words[i].find(".h") != string::npos || words[i].find(".cpp") != string::npos || words[i].find(".c") != string::npos)
            cppsCount++;
        //else
        else if (words[i] != "")
            objectsCount++;
    }
    words.clear();
    if (objectsCount != 0 && cppsCount == 0)
        return ONLY_OBJECTS;
    if (objectsCount == 0 && cppsCount != 0)
        return ONLY_CPPS;
    if (objectsCount != 0 && cppsCount != 0) {
        return MIXED;
    }
    return NO_INFO;
}

vector<string> SplitString(string& str) {
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
    return words;
}