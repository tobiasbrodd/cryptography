#include "parser.hpp"

Parser::Parser(int argc, const char** argv) {
    for (int i = 1; i < argc; i++) {
        this->tokens.push_back(string(argv[i]));
    }
}

bool Parser::findOption(const string& option) const {
    return find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}

const string& Parser::getOption(const string& option) const {
    vector<string>::const_iterator it = find(this->tokens.begin(), this->tokens.end(), option);
    if (it != this->tokens.end() && it++ != this->tokens.end()) {
        return *it;
    }

    static const string& empty_string("");
    return empty_string;
}

string Parser::read(const string& filename) {
    string contents;
    ifstream file(filename, ios::in | ios::binary);
    if (file.is_open()) {
        file.seekg(0, ios::end);
        contents.resize(file.tellg());
        file.seekg(0, ios::beg);
        file.read(&contents[0], contents.size());
        file.close();
    }

    return contents;
}

vector<string> Parser::read_words(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    if (file.is_open()) {
        string word;
        while (file >> word) {
            words.push_back(word);
        }
        file.close();
    }

    return words;
}