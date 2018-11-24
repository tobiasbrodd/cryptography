#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../parser.hpp"

#define FIRST_UPPER 65
#define LAST_UPPER 90
#define FIRST_NUMBER 48
#define LAST_NUMBER 49

using namespace std;

unordered_map<char, string> char_to_binary_char_v1 = {
    {'A', "00000"},
    {'B', "00001"},
    {'C', "00010"},
    {'D', "00011"},
    {'E', "00100"},
    {'F', "00101"},
    {'G', "00110"},
    {'H', "00111"},
    {'I', "01000"},
    {'J', "01000"},
    {'K', "01001"},
    {'L', "01010"},
    {'M', "01011"},
    {'N', "01100"},
    {'O', "01101"},
    {'P', "01110"},
    {'Q', "01111"},
    {'R', "10000"},
    {'S', "10001"},
    {'T', "10010"},
    {'U', "10011"},
    {'V', "10011"},
    {'W', "10100"},
    {'X', "10101"},
    {'Y', "10110"},
    {'Z', "10111"}
};

unordered_map<string, char> binary_char_to_char_v1 = {
    {"00000", 'A'},
    {"00001", 'B'},
    {"00010", 'C'},
    {"00011", 'D'},
    {"00100", 'E'},
    {"00101", 'F'},
    {"00110", 'G'},
    {"00111", 'H'},
    {"01000", 'I'},
    {"01000", 'J'},
    {"01001", 'K'},
    {"01010", 'L'},
    {"01011", 'M'},
    {"01100", 'N'},
    {"01101", 'O'},
    {"01110", 'P'},
    {"01111", 'Q'},
    {"10000", 'R'},
    {"10001", 'S'},
    {"10010", 'T'},
    {"10011", 'U'},
    {"10011", 'V'},
    {"10100", 'W'},
    {"10101", 'X'},
    {"10110", 'Y'},
    {"10111", 'Z'}
};

unordered_map<char, string> char_to_binary_char_v2 = {
    {'A', "00000"},
    {'B', "00001"},
    {'C', "00010"},
    {'D', "00011"},
    {'E', "00100"},
    {'F', "00101"},
    {'G', "00110"},
    {'H', "00111"},
    {'I', "01000"},
    {'J', "01001"},
    {'K', "01010"},
    {'L', "01011"},
    {'M', "01100"},
    {'N', "01101"},
    {'O', "01110"},
    {'P', "01111"},
    {'Q', "10000"},
    {'R', "10001"},
    {'S', "10010"},
    {'T', "10011"},
    {'U', "10100"},
    {'V', "10101"},
    {'W', "10110"},
    {'X', "10111"},
    {'Y', "11000"},
    {'Z', "11001"}
};

unordered_map<string, char> binary_char_to_char_v2 = {
    {"00000", 'A'},
    {"00001", 'B'},
    {"00010", 'C'},
    {"00011", 'D'},
    {"00100", 'E'},
    {"00101", 'F'},
    {"00110", 'G'},
    {"00111", 'H'},
    {"01000", 'I'},
    {"01001", 'J'},
    {"01010", 'K'},
    {"01011", 'L'},
    {"01100", 'M'},
    {"01101", 'N'},
    {"01110", 'O'},
    {"01111", 'P'},
    {"10000", 'Q'},
    {"10001", 'R'},
    {"10010", 'S'},
    {"10011", 'T'},
    {"10100", 'U'},
    {"10101", 'V'},
    {"10110", 'W'},
    {"10111", 'X'},
    {"11000", 'Y'},
    {"11001", 'Z'}
};

int mod(int x, int d) {
    return ((x % d) + d) % d;
}

string string_to_binary_string(int version, const string& s) {
    string binary_string = "";
    for (string::const_iterator it = s.begin(); it != s.end(); it++) {
        string binary_char = "";
        if (version == 1) {
            binary_char = char_to_binary_char_v1[*it];
        } else {
            binary_char = char_to_binary_char_v2[*it];
        }
        if (binary_char.length() != 0) {
            binary_string += binary_char;
        }
    }

    return binary_string;
}

string binary_string_to_string(bool invert, int version, const string& binary_string) {
    string s = "";
    for (string::const_iterator it = binary_string.begin(); it != binary_string.end(); it = it + 5) {
        string binary_char = "";
        for(string::const_iterator sub_it = it; sub_it != it + 5; sub_it++) {
            char c = *sub_it;
            if (invert) {
                c = LAST_NUMBER - (c - FIRST_NUMBER) ;
            }
            binary_char += c;
        }

        char c = 0;
        if (version == 1) {
            c = binary_char_to_char_v1[binary_char];
        } else {
            c = binary_char_to_char_v2[binary_char];
        }
        
        if (c != 0) {
            s += c;
        }
    }

    return s;
}

vector<string> encrypt(int version, const vector<string> text) {
    vector<string> encrypted_text;
    for(vector<string>::const_iterator it = text.begin(); it != text.end(); it++) {
        encrypted_text.push_back(string_to_binary_string(version, *it));
    }
    
    return encrypted_text;
}

vector<string> decrypt(bool invert, int version, const vector<string> text) {
    vector<string> decrypted_text;
    for(vector<string>::const_iterator it = text.begin(); it != text.end(); it++) {
        decrypted_text.push_back(binary_string_to_string(invert, version, *it));
    }
    
    return decrypted_text;
}

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        cout << "Usage: bacon [-e] [-d] [-i] [-v] <version> [-w] <word> [-f] <filename>" << endl;
        return 0;
    }

    Parser parser = Parser(argc, argv);
    bool encryption_option = !parser.findOption("-d");

    bool invert = parser.findOption("-i");

    int version = 1;

    if (parser.findOption("-v")) {
        version = stoi(parser.getOption("-v"));
    }

    vector<string> text;
    if (parser.findOption("-w")) {
        text.push_back(parser.getOption("-w"));
    } else if (parser.findOption("-f")) {
        string filename = parser.getOption("-f");
        text = parser.read_words(filename);
    }
    
    vector<string> message = (encryption_option) ? encrypt(version, text) : decrypt(invert, version, text);
    
    for(vector<string>::iterator it = message.begin(); it != message.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    return 0;
}
