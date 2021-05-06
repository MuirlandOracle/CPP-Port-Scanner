/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			portScan.h
*  Purpose:			Class definitions for the ScanPort and FingerprintPort task classes
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/


#pragma once
#ifndef PORTSCANFUNCS
#define PORTSCANFUNCS


#include "task.h"
#include <mutex>
#include <string>
#include <atomic>
#include <set>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>


using std::mutex;
using std::set;
using std::map;
using std::string;
using std::atomic;




//Prototype for the ScanPort task -- used to get an initial reading on open/closed ports
class ScanPort : public Task
{
public:
	ScanPort(const sf::IpAddress& address, const int port, set<int>* openPorts, atomic<int>& openPortsNum, atomic<int>& closedPortsNum)
		: address_(address), port_(port), openPorts_(openPorts), openPortsNum_(openPortsNum), closedPortsNum_(closedPortsNum)
	{}

	void run();
private:
	//Store information about the Address to be scanned
	const sf::IpAddress address_;
	const int port_;
	//Get a handle on the openPorts set
	set<int>* openPorts_;
	//Set the connect timeout to be 100ms
	sf::Time timeout_ = sf::milliseconds(100);
	//Get references to the openPorts atomics
	atomic<int>& openPortsNum_;
	atomic<int>& closedPortsNum_;
};

//Prototype for the FingerprintPort task -- used to try pulling a raw fingerprint from established open ports
class FingerprintPort : public Task
{
public:
	FingerprintPort(const sf::IpAddress& address, const int port, map<const int, string>* fingerprints)
		: address_(address), port_(port), fingerprints_(fingerprints)
	{}

	void run();
private:
	//Store information about the Address to be scanned
	const sf::IpAddress address_;
	const int port_;
	//Store a reference to the map output
	map<const int, string>* fingerprints_;
};

#endif