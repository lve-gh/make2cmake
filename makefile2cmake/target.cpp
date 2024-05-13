#include "target.hpp"


    void Target::ClearTarget() {
        this->name = "";
        if (this->cpps.size() != 0) {
            this->cpps.clear();
        }
        if (this->headers.size() != 0) {
            this->headers.clear();
        }

    }

    void Target::AddToCPP(string cpp) {
        this->cpps.emplace_back(cpp);
        this->empty = false;
    }

    void Target::AddToHeader(string header) {
        this->headers.emplace_back(header);
        this->empty = false;
    }
    void Target::SetName(string name) {
        this->name = name;
        this->empty = false;
    }

    void Target::SetLinks(string links) {
        this->links = links;
        this->empty = false;
    }

    vector<string> Target::GetCPPS() {
        return this->cpps;
    }

    vector<string> Target::GetHeaders() {
        return this->headers;
    }

    string Target::GetName() {
        return this->name;
    }

    string Target::GetLinks() {
        return this->links;
    }

    bool Target::isEmpty() {
        return this->empty;
    }

    bool Target::isCPPSEmty() {
        if (this->cpps.size() == 0)
            return true;
        return false;
    }

    bool Target::isHeadersEmpty() {
        if (this->headers.size() == 0)
            return true;
        return false;
    }

    bool Target::isExistInCPP(string str) {
        if (this->cpps.size() == 0)
            return false;
        for (int i = 0; i < cpps.size(); i++) {
            if (str == cpps[i])
                return true;
        }
        return false;
    }

    bool Target::isExistInHeader(string str) {
        if (this->headers.size() == 0)
            return false;
        for (int i = 0; i < headers.size(); i++) {
            if (str == headers[i])
                return true;
        }
        return false;
    }

//};