/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			Source.cpp
*  Purpose:			Main Driver
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#include "CLI11.hpp"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <chrono>
#include <atomic>
#include <fstream>
#include "argParseHelpers.h"
#include "farm.h"
#include "portScan.h"

using std::string;
using std::cout;
using std::set;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::atomic;
using std::map;

typedef std::chrono::steady_clock timer;


//Main driver for the program
int main(int argc, char** argv) {
	//Arg variable initialisation
	sf::IpAddress ip;
	string portsRaw = "None";
	set<int> portRange;
	bool serviceEnum = 0;
	bool format[2] = { 0, 0 };
	//Output variable initialisation
	set<int> openPortsList;
	map<const int, string> fingerprints;
	atomic<int> openPortsNum;
	atomic<int> closedPortsNum;
	//Multi-threaded Farm Initialisation
	Farm farm;
	
	//Argparsing
	CLI::App app{ "IPv4 TCP Portscanner | AG" };
	app.allow_windows_style_options(true);
	app.add_option<sf::IpAddress, string>("IP", ip, "IP to scan")->required()->type_name("IPv4")->check(ipCheck);
	app.add_option("-p,--ports", portsRaw, "Port range to scan (comma separated, hyphen ranges)")->required()->type_name("Port List");
	app.add_flag("-f,--fingerprint", serviceEnum, "Attempt port fingerprinting");
	app.add_flag("--long", format[0], "Output results in long-format (default)");
	app.add_flag("-l,--list", format[1], "Output results as a list. Overrules long-format unless both are specified");
	CLI11_PARSE(app, argc, argv);

	//Validate the ports and output to the portRange set: string => set<int>
	parsePorts(portsRaw, &portRange);



	cout << "\nScanning " << ip.toString() << "...\n";


	timer::time_point start;
	timer::time_point end;
	std::ofstream results("results.csv");
	for (int j = 0; j < 10000; j++) {
		cout << "Iteration: " << j << "\n";
		//Scan the parsed ports (one port per task). Pass in a reference to the openPortsList for output, as well as the atomic open/closed port numbers
		for (int i : portRange) {
			farm.add_task(new ScanPort(ip, i, &openPortsList, openPortsNum, closedPortsNum));
		}
		//Start the clock for the port-scan
		start = timer::now();
		//Run the scan
		farm.run();
		//Stop the clock
		end = timer::now();
		results << duration_cast<milliseconds>(end - start).count() << ",";
	}
	results << "\b ";
	results.close();


	//Output the number of open and closed ports, plus the time taken to scan
	cout << "\n" << portRange.size() << " ports scanned in " << duration_cast<milliseconds>(end - start).count() << "ms\n";
	cout << "Open Ports: " << openPortsNum << "\n" << "Closed Ports: " << closedPortsNum << "\n\n";
	

	if (openPortsList.size() == 0) {
		exit(0);
	}

	cout << "===== Results =====" << "\n";
	//Handle the output format
	//	If long form has been explicitly requested, or list form has not been requested, output in long form
	if (format[0] || !format[1]) {
		for (int i : openPortsList) {
			cout << "Port " << i << " is open\n";
		} cout << "\n";
	}
	//	If list form has been requested, output in list form
	if (format[1]) {
		for (int i : openPortsList) {
			cout << i << ",";
		} cout << "\b \n";
	}


	

	//If fingerprinting has been requested, start the farm again
	if (serviceEnum) {
		cout << "\n\nFingerprinting...\r";
		//Add tasks to the farm, entering ip and port, then accepting string output into a map<int, string> structure
		for (int i : openPortsList) {
			farm.add_task(new FingerprintPort(ip, i, &fingerprints));
		}
		farm.run();

		//Output the fingerprints in order
		cout << "===== Fingerprinting =====\t\n";
		for (auto i : openPortsList) {
			cout << fingerprints.at(i) << "\n";
		}
	}

}