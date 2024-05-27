#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

class Target {
private:
    bool empty = true;
    vector<string> cpps;
    vector<string> headers;
    string name = "";
    string links = "";
public:
    void SetName(string name);
    void SetLinks(string links);
    vector<string> GetCPPS();
    vector<string> GetHeaders();
    string GetName();
    string GetLinks();
};
