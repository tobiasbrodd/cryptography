#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include "../parser.hpp"

#define MAX 127
#define LENGTH 3

using namespace std;

struct Key {
    unsigned long int n;
    unsigned long int k;
    Key(unsigned long n, unsigned long k) : n(n), k(k) {}
};

int digits(unsigned long n) {
    if (n == 0) return 1;
    int d = 0;
    while (n > 0) {
        n /= 10;
        d++;
    }
    return d;
}

unsigned long modpow(unsigned long base, unsigned long exp, unsigned long mod) {
    base %= mod;
    unsigned long result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

int get_blocksize(const Key& key) {
    int blocksize = 0;
    unsigned long max = MAX;
    while (max < key.n && max < ULONG_MAX) {
        max *= 1001;
        blocksize++;
    }

    return blocksize;
}

vector<unsigned long> text_to_blocks(const Key& key, const string& text, int b = 0) {
    size_t blocksize = (b > 0) ? b : get_blocksize(key);
    vector<unsigned long> blocks;
    int t, d;
    string s, m;

    size_t i = 0;
    while (i < text.length()) {
        m = "";
        for (size_t j = 0; j < blocksize && i < text.length(); j++) {
            t = text[i];
            s = "";
            d = LENGTH - digits(t);
            while (d > 0) {
                s += "0";
                d--;
            }
            s += to_string((int) text[i]);
            m += s;
            i++;
        }
        blocks.push_back(stoul(m));
    }

    return blocks;
}

string blocks_to_text(const Key& key, const vector<unsigned long> blocks, int b = 0) {
    size_t blocksize = (b > 0) ? b : get_blocksize(key);
    string text, s;
    size_t d;
    
    for(vector<unsigned long>::const_iterator it = blocks.begin(); it != blocks.end(); it++) {
        s = to_string(*it);
        d = blocksize * LENGTH - s.length();
        while (d > 0) {
            s.insert(s.begin(), '0');
            d--;
        }
        for (size_t i = 0; i < s.length(); i += LENGTH) {
            text += stoi(s.substr(i, LENGTH));
        }
    }

    return text;
}

string encrypt(const Key& key, const string& text) {
    string encrypted_text;
    vector<unsigned long> blocks = text_to_blocks(key, text);
    for (vector<unsigned long>::iterator it = blocks.begin(); it != blocks.end(); it++) {
        encrypted_text += to_string(modpow(*it, key.k, key.n));
        encrypted_text += " ";
    }
    encrypted_text.erase(encrypted_text.end());
    
    return encrypted_text;
}

string decrypt(const Key& key, const string& text) {
    vector<unsigned long> blocks;
    string encrypted_string;
    istringstream ss(text);
    while (ss >> encrypted_string) {
        blocks.push_back(modpow(stoul(encrypted_string), key.k, key.n));
    }
    
    return blocks_to_text(key, blocks);
}

Key get_key(const Parser& parser, bool encryption_option) {
    unsigned long n = stoul(parser.getOption("-n"));
    unsigned long k = (encryption_option) ? stoul(parser.getOption("-e")) : stoul(parser.getOption("-d")) ;
    return Key(n, k);
}

int main(int argc, const char *argv[]) {
    if (argc < 7) {
        cout << "Usage: rsa [-e] <enryption_key> [-d] <decryption_key> [-n] <n> [-w] <word> [-f] <filename>" << endl;
        return 0;
    }

    Parser parser = Parser(argc, argv);
    bool encryption_option = parser.findOption("-e");
    bool decryption_option = parser.findOption("-d");
    bool n_option = parser.findOption("-n");

    if (!n_option || !(encryption_option || decryption_option)) {
        cout << "Usage: rsa [-e] <enryption_key> [-d] <decryption_key> [-n] <n> [-w] <word> [-f] <filename>" << endl;
        return 0;
    }

    Key key = get_key(parser, encryption_option);

    string text;
    if (parser.findOption("-w")) {
        text = parser.getOption("-w");
    } else if (parser.findOption("-f")) {
        string filename = parser.getOption("-f");
        text = parser.read(filename);
    }
    
    string message = (encryption_option) ? encrypt(key, text) : decrypt(key, text);

    cout << message << endl;

    return 0;
}
