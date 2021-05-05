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
#include <atomic>


using std::string;
using std::cout;
using std::set;
using std::stringstream;
using std::getline;
using std::stoi;
using std::to_string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::atomic;


typedef std::chrono::steady_clock timer;





int main(int argc, char** argv) {
	//Argparsing
	CLI::App app{ "IPv4 TCP Portscanner | AG" };
	sf::IpAddress ip;
	string portsRaw= "None";
	set<int> portRange;
	set<int> openPortsList;
	atomic<int> openPortsNum;
	atomic<int> closedPortsNum;
	bool serviceEnum = 0;
	app.add_option<sf::IpAddress, string>("IP", ip, "IP to scan")->required()->type_name("IPv4")->check(ipCheck);
	app.add_option("-p,--ports", portsRaw, "Port range to scan (comma separated, hyphen ranges)")->required()->type_name("Port List");
	app.add_flag("-f,--fingerprint", serviceEnum, "Attempt port fingerprinting");
	CLI11_PARSE(app, argc, argv);

	parsePorts(portsRaw, &portRange);


	Farm farm;

	timer::time_point start = timer::now();
	for (int i : portRange) {
		farm.add_task(new ScanPort(ip, i, &openPortsList, openPortsNum, closedPortsNum));
	}
	farm.run();
	timer::time_point end = timer::now();

	for (int i : openPortsList) {
		cout << "Port " << i << " is open\n";
	}


	cout << "\nOpen Ports: " << openPortsNum << "\n" <<  "Closed Ports: " << closedPortsNum << "\n";
	cout << "\n" << portRange.size() << " ports scanned in " << duration_cast<milliseconds>(end - start).count() << "ms\n";

	//Farm farm2;
	if (serviceEnum) {
		for (int i : openPortsList) {
			farm.add_task(new FingerprintPort(ip, i));
		}
		farm.run();
	}

}