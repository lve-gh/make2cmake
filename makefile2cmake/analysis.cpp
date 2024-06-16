#include "analysis.hpp"

using std::filesystem::recursive_directory_iterator;
using std::filesystem::begin;


bool ContainsAsWord(std::string& str, std::string& wordInString) {
    std::vector<std::string> words;
    boost::split(words, str, boost::is_any_of(" \t\n\r"), boost::token_compress_on);

    for (const auto& word : words) {
        if (word == wordInString) {
            return true;
        }
    }
    return false;
}

bool ContainsAsSubstring(const std::string& str, const std::string& substring) {
    return boost::algorithm::contains(str, substring);
}


int countWords(std::string& str) {
    std::vector<std::string> words;
    boost::algorithm::split(words, str, boost::is_any_of(" "));
    return static_cast<int>(words.size());
}

string DefineCompiler(vector<string> input, vector<string> makefile) {
    for (size_t i = 0; i < input.size(); i++) {
        if (IsTarget(input[i]) && i != (input.size() - 1)&& input[i + 1].length() > 0) {
            std::vector<std::string> words;
            boost::split(words, input[i + 1], boost::is_any_of(" \t\n\r"), boost::token_compress_on);
            return words[0];
        }

        boost::algorithm::erase_all(input[i], " \t\r\n");

        if (input[i].length() > 3) {
            if ((input[i].substr(0, 3)) == "CC=") {
                return (input[i].substr(3, input[i].size() - 1));
            }
        }

    }
    return "";
}

string TransformLinksInVars(string input, vector<string> makefile) {
    vector<pair<string, string>> vars;
    for (int i = 0; i < makefile.size(); i++) {
        if (StringAnalysis(makefile[i])) {
            size_t pos = makefile[i].find("=");
            if (pos != string::npos)
            {
                pair<string, string> varTemp;
                varTemp = make_pair (makefile[i].substr(0, pos), (makefile[i].substr(pos + 1)));
                vars.emplace_back(varTemp);
            }
        }
    }
    for (int i = 0; i < vars.size(); i++) {
        if (input == "${" + vars[i].first + "}")
            return vars[i].second;
    }
    return "";
}


std::vector<std::string> ExtractFlags(std::string str, vector<string> makefile) {
    std::vector<std::string> words;
    std::vector<std::string> result;
    boost::algorithm::split(words, str, boost::is_any_of(" "));
    for (int i = 0; i < words.size(); i++)
    
    for (const auto& word : words) {
        if ((TransformLinksInVars(word, makefile).size() > 0 && TransformLinksInVars(word, makefile)[0] == '-') || (TransformLinksInVars(word, makefile).size() > 2 && TransformLinksInVars(word, makefile)[0] == '$' && TransformLinksInVars(word, makefile)[TransformLinksInVars(word, makefile).length() - 1] == '}')) {
            result.push_back(TransformLinksInVars(word, makefile));
        }
    }
    return result;
}

string RemoveDuplicateWords(string& input_string) {
    std::vector<std::string> words;
    boost::split(words, input_string, boost::is_any_of(" "));

    std::set<std::string> unique_words(words.begin(), words.end()); 

    std::string result = boost::algorithm::join(unique_words, " "); 

    return result;
}


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
    for (size_t i = 0; i < words.size(); i++) {
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
    for (size_t i = 0; i < words.size(); i++) {
        if (words[i].find(word) != string::npos) {
            wordTemp = words[i];
            words.clear();
            return wordTemp;
        }
    }
    words.clear();
    return "";
}

string ReturnArgsFromString(string& str) {
    vector<string> words = SplitString(str);
    string word;
    for (size_t i = 0; i < words.size(); i++) {
        if (words[i].find("./") != string::npos) {
            word = words[i];
            words.clear();
            return word;
        }
    }
    words.clear();
    return "";
}

string ReturnCommandFromString(string& str) {
    vector<string> words = SplitString(str);
    if (words.size() != 0)
        return words[0];
    else
        return "";
}

string ReturnCPPFromString(string& str) {
    vector<string> words = SplitString(str);
    string word;
    for (size_t i = 0; i < words.size(); i++) {
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
    for (size_t i = 0; i < words.size(); i++) {
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