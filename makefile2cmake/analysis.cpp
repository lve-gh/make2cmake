#include "analysis.hpp"

bool IsCPP(string word) {
    std::istringstream iss(word);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (token == "cpp" || token == "c") {
            return true;
        }
    }
    return false;
}

bool IsHeader(string word) {
    std::istringstream iss(word);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (token == "hpp" || token == "h") {
            return true;
        }
    }
    return false;
}

bool IsEqualsSign(string word) {
    if (word.find("=") != std::string::npos && !(word.find(":=") != std::string::npos))
        return true;
    return false;
}

bool IsCommand(string word) {
    std::istringstream iss(word);
    std::string token;
    if (word.find("./") != std::string::npos)
        return true;
    return false;
}

bool IsTarget(string word) {
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

string IsContains(string word, vector<string> words) {
    for (int i = 0; i < words.size(); i++) {
        if (words[i].find(word) != string::npos)
            return words[i];
    }
    return "";
}