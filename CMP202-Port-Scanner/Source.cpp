#include "CLI11.hpp"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include "argParseHelpers.h"
#include "farm.h"
#include "portScan.h"
#include <set>
#include <map>
#include <chrono>
#include <atomic>


using std::string;
using std::cout;
using std::set;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::atomic;
using std::map;


typedef std::chrono::steady_clock timer;





int main(int argc, char** argv) {
	//Initialisation
	sf::IpAddress ip;
	string portsRaw = "None";
	set<int> portRange;
	set<int> openPortsList;
	map<const int, string> fingerprints;
	atomic<int> openPortsNum;
	atomic<int> closedPortsNum;
	bool serviceEnum = 0;
	bool format[2] = { 0, 0 };
	
	//Argparsing
	CLI::App app{ "IPv4 TCP Portscanner | AG" };
	app.allow_windows_style_options(true);
	app.add_option<sf::IpAddress, string>("IP", ip, "IP to scan")->required()->type_name("IPv4")->check(ipCheck);
	app.add_option("-p,--ports", portsRaw, "Port range to scan (comma separated, hyphen ranges)")->required()->type_name("Port List");
	app.add_flag("-f,--fingerprint", serviceEnum, "Attempt port fingerprinting");
	app.add_flag("--long", format[0], "Output results in long-format (default)");
	app.add_flag("-l,--list", format[1], "Output results as a list. Overrules long-format unless both are specified");
	CLI11_PARSE(app, argc, argv);

	parsePorts(portsRaw, &portRange);

	cout << "\n\nScanning...\r";
	Farm farm;

	timer::time_point start = timer::now();
	for (int i : portRange) {
		farm.add_task(new ScanPort(ip, i, &openPortsList, openPortsNum, closedPortsNum));
	}
	farm.run();
	timer::time_point end = timer::now();


	if (format[0] || !format[1]) {
		cout << "Open Ports:\t\n";
		for (int i : openPortsList) {
			cout << "Port " << i << " is open\n";
		} cout << "\n";
	}
	if (format[1]) {
		cout << "Open Ports List:\t\n";
		for (int i : openPortsList) {
			cout << i << ",";
		} cout << "\b \n";
	}

	cout << "\n\nOpen Ports: " << openPortsNum << "\n" <<  "Closed Ports: " << closedPortsNum << "\n";
	cout << "\n" << portRange.size() << " ports scanned in " << duration_cast<milliseconds>(end - start).count() << "ms\n";


	if (serviceEnum) {
		cout << "\n\nFingerprinting...\r";
		for (int i : openPortsList) {
			farm.add_task(new FingerprintPort(ip, i, &fingerprints));
		}
		farm.run();
		cout << "Fingerprinting:\t\n";
		for (auto i : openPortsList) {
			cout << fingerprints.at(i) << "\n";
		}
	}

}