#pragma once
#include "argParseHelpers.h"
#include <string>
#include <sstream>
#include <assert.h>
#include <vector>
#include <iostream>
#include <stdlib.h>

using std::string;
using std::cout;
using std::stringstream;
using std::vector;
using std::stoi;
using std::exit;


int portCheck(string portRaw) {
	int port;
	try {
		port = stoi(portRaw);
		assert(port > 0 && port < 65535);
	}
	catch (const std::exception& e) {
		return -1;
	}
	return port;
}


void parsePorts(string& ports, vector<int>* portList) {
	string temp;
	int convertedPort;
	stringstream ss(ports);
	while (getline(ss, temp, ',')) {
		if (temp.find("-") == string::npos) {
			convertedPort = portCheck(temp);
			if (convertedPort < 0) {
				cout << "Invalid port: " << temp << "\n";
				exit(1);
			}
			portList->push_back(convertedPort);
		}
		else {
			continue;
		}
	}


}