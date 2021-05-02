#include "CLI11.hpp"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "argParseHelpers.h"
#include <set>

using std::string;
using std::cout;
using std::set;
using std::vector;
using std::stringstream;
using std::getline;
using std::stoi;


//Connection Timeout
sf::Time timeout = sf::milliseconds(100);



//bool scanPort(const std::string& address, int port) {
bool scanPort(const sf::IpAddress& address, int port) {
	cout << address << "\n";
	return (sf::TcpSocket().connect(address, port, timeout) == sf::Socket::Done);
}



int main(int argc, char** argv) {
	//Argparsing
	CLI::App app{ "IPv4 TCP Portscanner | AG" };
	sf::IpAddress ip;
	string portsRaw= "None";
	set<int> portRange;
	app.add_option<sf::IpAddress, string>("IP", ip, "IP to scan")->required()->type_name("IPv4")->check(ipCheck);
	app.add_option("-p,--ports", portsRaw, "Port range to scan")->required();
	CLI11_PARSE(app, argc, argv);
	parsePorts(portsRaw, &portRange);

	for (int i : portRange) {
		if (scanPort(ip, i)) {
			cout << "Port " << i << " is open\n";			
		}
		else {
			cout << "Port " << i << " is closed\n";
		}
		
	}
	
}