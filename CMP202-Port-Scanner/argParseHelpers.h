/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			argParseHelpers.h
*  Purpose:			Prototypes for the argparse check functions
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#pragma once
#include <string>
#include <vector>
#include <set>

using std::set;
using std::string;


//Implementations in argParseHelpers.cpp
int portCheck(string);
void parsePorts(string&, set<int>*);
string ipCheck(string);