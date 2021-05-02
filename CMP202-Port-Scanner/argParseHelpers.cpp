#pragma once
#include "argParseHelpers.h"
#include <string>
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
		assert(port > 0 && port < 65535);
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
			//convertedPort = portCheck(temp);
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
	int count = 0;
	int octet;
	string octetRaw;
	stringstream ss(ip);
	while (getline(ss, octetRaw, '.')) {
		++count;
		if (count > 4) {
			break;
		}
		try {
			octet = stoi(octetRaw);
			if (octet < 0 || octet > 255) {
				throw (std::exception());
			}
		}
		catch (std::exception& e) {
			return err;
		}
	}
	if (count != 4) {
		return err ;
	}
	return "";
}