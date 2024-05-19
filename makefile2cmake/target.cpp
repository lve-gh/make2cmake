#include "target.hpp"

    void Target::SetName(string &name) {
        this->name = name;
        this->empty = false;
    }

    void Target::SetLinks(string &links) {
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
