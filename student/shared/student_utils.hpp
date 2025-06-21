#ifndef STUDENT_UTILS_HPP
#define STUDENT_UTILS_HPP

#include <vector>
#include <string>
#include <sstream>
using namespace std;

// Generic line splitter with default delimiter '|'
inline vector<string> splitLine(const string& line, char delimiter = '|') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

#endif
