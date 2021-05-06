/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			argParseHelpers.cpp
*  Purpose:			Checks for the argparser -- used to validate arguments
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

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

//Checks to see if a port is valid (i.e. an integer between 1 and 65535)
int portCheck(string portRaw) {
	int port;
	try {
		port = stoi(portRaw);
		if (port < 1 || port > 65535) {
			throw std::exception();
		}
	}
	catch (const std::exception& e) {
		//If a port is invalid, just exit -- up to the user to specify valid ports
		cout << "Invalid port: " << portRaw << "\n";
		exit(1);
	}
	return port;
}

//Attempt to parse the raw port string into a set of integers
void parsePorts(string& ports, std::set <int>* portList) {
	//Temporary string used to store a substring split at commas
	string temp;
	int convertedPort;
	//Get a stringstream for the raw port string
	stringstream ss(ports);
	//Split the raw port string at commas and loop through it
	while (getline(ss, temp, ',')) {
		//If there is no hyphen in the string (i.e. it's not meant to be a port range),
		//	check that it's valid and add it to the set
		if (temp.find("-") == string::npos) {
			portList->insert(portCheck(temp));
		}
		else {
			//Stringstream to split the temporary string at hyphens
			stringstream hyphen(temp);
			//Should only be two ports specified here, so creating arrays for the raw and converted ports to accommodate
			string portsRaw[2];
			int ports[2];
			//Loop through both positions in the array to get the ports
			for (int i = 0; i < 2; i++) {
				getline(hyphen, portsRaw[i], '-');
				if (portsRaw[i] == "") {
					//Set the port to a default value (1-65535) if a port isn't explicitly specified
					// Allows for things like "-100" to scan the first 100 ports, or "-" to scan all ports
					ports[i] = 65534 * i + 1;
				}
				else {
					ports[i] = portCheck(portsRaw[i]);
				}
			}
			//Exit if the port range has been specified in the wrong order
			if (ports[0] > ports[1]){
				cout << "Invalid port range: " << temp << "\n";
				exit(1);
			}

			//If all good, populate the set with all the ports within the range
			for (int i = ports[0]; i <= ports[1]; i++) {
				portList->insert(i);
			}
			
		}
	}


}

//Check integrated with CLI11 to confirm that the IP address is valid. 
//	If it's not a valid IP/Domain, sf::IpAddress will slot in a default 0.0.0.0
string ipCheck(string ip) {
	string err = "Invalid IPv4 Address (" + ip + ")";
	if (sf::IpAddress(ip).toString() == "0.0.0.0") {
		return err;
	}
	return "";
}