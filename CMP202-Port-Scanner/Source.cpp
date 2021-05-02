#include "CLI11.hpp"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "argParseHelpers.h"

using std::string;
using std::cout;
using std::vector;
using std::stringstream;
using std::getline;
using std::stoi;


//Connection Timeout
sf::Time timeout = sf::milliseconds(100);



bool scanPort(const std::string& address, int port) {
	return (sf::TcpSocket().connect(address, port, timeout) == sf::Socket::Done);
}



int main(int argc, char** argv) {
	//Argparsing
	CLI::App app{ "Simple Portscanner | AG" };
	
	string ip = "None";
	string portsRaw= "None";
	vector<int> portRange;
	app.add_option("IP", ip, "IP to scan")->required();
	app.add_option("-p,--ports", portsRaw, "Port range to scan")->required();
	CLI11_PARSE(app, argc, argv);
	cout << ip << "\n";
	parsePorts(portsRaw, &portRange);
	for (auto port : portRange) {
		cout << port << "\n";
	}


	cout << scanPort("127.0.0.1", 443) << "\n";
	
}