#include "CLI11.hpp"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "argParseHelpers.h"
#include "farm.h"
#include "portScan.h"
#include <set>
#include <chrono>


using std::string;
using std::cout;
using std::set;
using std::stringstream;
using std::getline;
using std::stoi;
using std::to_string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


typedef std::chrono::steady_clock timer;


//Globals
set<int> openPorts;
/*sf::Time timeout = sf::milliseconds(100);

bool scanPort(const sf::IpAddress& address, int port) {
	return (sf::TcpSocket().connect(address, port, timeout) == sf::Socket::Done);
}*/


string fingerprintPort(const sf::IpAddress& address, int port) {
	const int resLength = 300;
	char data[resLength];
	string header;
	header = "===== " + address.toString() + ":" + to_string(port) + " =====\n";
	string returnVal = "";
	std::size_t received;
	sf::TcpSocket sock;
	sock.connect(address, port);
	sock.send("Fingerprint", 12);
	if (sock.receive(data, resLength, received) != sf::Socket::Done) {
		return header + "Couldn't retrieve header\n";
	}
	for (int i = 0; i < received; i++) {returnVal += (data[i]);}
	return header + returnVal + "\n";
}




int main(int argc, char** argv) {
	//Argparsing
	CLI::App app{ "IPv4 TCP Portscanner | AG" };
	sf::IpAddress ip;
	string portsRaw= "None";
	set<int> portRange;
	bool serviceEnum = 0;
	app.add_option<sf::IpAddress, string>("IP", ip, "IP to scan")->required()->type_name("IPv4")->check(ipCheck);
	app.add_option("-p,--ports", portsRaw, "Port range to scan (comma separated, hyphen ranges)")->required()->type_name("Port List");
	app.add_flag("-f,--fingerprint", serviceEnum, "Attempt port fingerprinting");
	CLI11_PARSE(app, argc, argv);

	parsePorts(portsRaw, &portRange);


	Farm farm;

	timer::time_point start = timer::now();
	for (int i : portRange) {
		farm.add_task(new ScanPort(ip, i));
	}
	farm.run();
	timer::time_point end = timer::now();

	cout << "\n\n" << portRange.size() << " ports scanned in " << duration_cast<milliseconds>(end - start).count() << "ms\n";
	
	/*if (serviceEnum) {
		cout << "\n\nFingerprinting:\n" << fingerprintPort("google.com", 443) << "\n";
	}*/
}