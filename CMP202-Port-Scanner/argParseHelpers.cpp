#pragma once
#include "argParseHelpers.h"
#include <string>
#include <SFML/Network.hpp>
#include <sstream>
#include <assert.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <set>

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
		if (port < 0 || port > 65535) {
			throw std::exception();
		}
	}
	catch (const std::exception& e) {
		cout << "Invalid port: " << portRaw << "\n";
		exit(1);
	}
	return port;
}


void parsePorts(string& ports, std::set <int>* portList) {
	string temp;
	int convertedPort;
	stringstream ss(ports);
	while (getline(ss, temp, ',')) {
		if (temp.find("-") == string::npos) {
			portList->insert(portCheck(temp));
		}
		else {
			stringstream hyphen(temp);
			string portsRaw[2];
			int ports[2];
			for (int i = 0; i < 2; i++) {
				getline(hyphen, portsRaw[i], '-');
				ports[i] = portCheck(portsRaw[i]);
			}
			if (ports[0] > ports[1]){
				cout << "Invalid port range: " << temp << "\n";
				exit(1);
			}

			for (int i = ports[0]; i <= ports[1]; i++) {
				portList->insert(i);
			}
			
		}
	}


}


string ipCheck(string ip) {
	string err = "Invalid IPv4 Address (" + ip + ")";
	if (sf::IpAddress(ip).toString() == "0.0.0.0") {
		return err;
	}
	return "";
}