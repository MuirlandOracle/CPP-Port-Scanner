#pragma once
#include <string>
#include <vector>
#include <set>

using std::set;
using std::string;

int portCheck(string);
void parsePorts(string&, set<int>*);
string ipCheck(string);