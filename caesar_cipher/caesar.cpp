#include <iostream>
#include <string>
#include <vector>
#include "../parser.hpp"

#define FIRST_UPPER 65
#define LAST_UPPER 90
#define FIRST_LOWER 97
#define LAST_LOWER 122

using namespace std;

int mod(int x, int d) {
    return ((x % d) + d) % d;
}

string encrypt_string(int key, const string& s) {
    string encrypted_string = "";
    for (string::const_iterator it = s.begin(); it != s.end(); it++) {
        if (*it < FIRST_UPPER || *it > LAST_LOWER) {
            encrypted_string += *it;
        } else if (isupper(*it)) {
            encrypted_string += mod((*it + key - FIRST_UPPER), (LAST_UPPER - FIRST_UPPER + 1)) + FIRST_UPPER;
        } else if (islower(*it)) {
            encrypted_string += mod((*it + key - FIRST_LOWER), (LAST_LOWER - FIRST_LOWER + 1)) + FIRST_LOWER;
        } else {
            encrypted_string += *it + key;
        }
    }
    return encrypted_string;
}

vector<string> encrypt(int key, const vector<string> text) {
    vector<string> encrypted_text;
    for (vector<string>::const_iterator it = text.begin(); it != text.end(); it++) {
        encrypted_text.push_back(encrypt_string(key, *it));
    }
    
    return encrypted_text;
}

vector<string> decrypt(int key, const vector<string> text) {
    return encrypt(-key, text);
}

int main(int argc, const char *argv[]) {
    if (argc < 6) {
        cout << "Usage: caesar [-e] [-d] [-k] <key> [-w] <word> [-f] <filename>" << endl;
        return 0;
    }

    Parser parser = Parser(argc, argv);
    bool encryption_option = !parser.findOption("-d");

    int key = stoi(parser.getOption("-k"));
    vector<string> text;
    if (parser.findOption("-w")) {
        text.push_back(parser.getOption("-w"));
    } else if (parser.findOption("-f")) {
        string filename = parser.getOption("-f");
        text = parser.read_words(filename);
    }
    
    vector<string> message = (encryption_option) ? encrypt(key, text) : decrypt(key, text);

    
    for(vector<string>::iterator it = message.begin(); it != message.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    return 0;
}
