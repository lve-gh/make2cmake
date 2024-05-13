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
    void ClearTarget();
    void AddToCPP(string cpp);
    void AddToHeader(string header);
    void SetName(string name);
    void SetLinks(string targets);
    vector<string> GetCPPS();
    vector<string> GetHeaders();
    string GetName();
    string GetLinks();
    bool isEmpty();
    bool isCPPSEmty();
    bool isHeadersEmpty();
    bool isExistInCPP(string str);
    bool isExistInHeader(string str);
};
