#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool fileExists(const string& filename);

bool openFileSafely(const string& filename, ifstream& fileRef);

int getValidatedChoice(int min, int max);

#endif
