#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Parser {
private:
    vector<string> tokens;
public:
    Parser(int argc, const char** argv);

    bool findOption(const string& option) const;
    const string& getOption(const string& option) const;
    string read(const string& filename);
    vector<string> read_words(const string& filename);
};

#endif