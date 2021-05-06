/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			portScan.cpp
*  Purpose:			Implementation of ScanPort and FingerPrint port classes
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/


#pragma once
#include "task.h"
#include "portScan.h"
#include <mutex>
#include <chrono>
#include <string>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

using std::unique_lock;
using std::mutex;
using std::string;
using std::to_string;
using std::pair;
using std::cout;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


typedef std::chrono::steady_clock timeout;

//Global mutexes for protecting the openPorts set and fingerprint map
mutex openPortsMutex;
mutex mapLock;


//Driver for the ScanPort task
void ScanPort::run() {
	//Check to see if the desired port could be connected to successfully
	if (sf::TcpSocket().connect(address_, port_, timeout_) == sf::Socket::Done) {
		//If the port was successfully connected to, open a unique lock and insert the port into the set of open ports
		unique_lock<mutex> lock(openPortsMutex);
		openPorts_->insert(port_);
		//Add one to the atomic<int> of open ports
		++openPortsNum_;
	}
	else {
		//Add one to the atomic<int> of closed ports, should there have been no response
		++closedPortsNum_;
	}
}


//Driver for the FingerprintPort task
void FingerprintPort::run() {
	//Define how many bytes to accept from the server and initialise a char array of that length
	const int resLength = 300;
	char data[resLength];
	
	//Initialise the header and response string variables to construct a return value
	string header = "~~~ " + address_.toString() + ":" + to_string(port_) + " ~~~\n";
	string res = "";
	//Initialise a variable to contain the length of return data (to be used if less than 300 bytes)
	std::size_t received;

	//Initialise the socket connection and connect to the target
	sf::TcpSocket sock;
	sock.connect(address_, port_);
	

	//Send a fixed string with a newline to prompt a response (required by some protocols)
	sock.send("Fingerprint\n", 13);

	//Set the socket to not block the thread whilst listening
	sock.setBlocking(false);


	//Initialise a timeout for receiving data from the service (avoids the program hanging if the service doesn't return anything
	timeout::time_point start = timeout::now();

	//For duration of the timeout (5 seconds), check to see if the socket has received anything
	do {
		//If a response has been received, format the data into a string stored at res and break out of the loop
		if (sock.receive(data, resLength, received) == sf::Socket::Done) {
			for (int i = 0; i < received; i++) { res += data[i]; }
			break;
		}
	} while (duration_cast<milliseconds>(timeout::now() - start).count() < 5000);

	//Check to see if a response was received. If not, set the response to a negative
	if (res == "") {
		res = "Couldn't retrieve header\n";
	}

	//Prepend the header to the response
	res = header + res;

	//Open a lock on the fingerprints map and insert the port and response
	unique_lock<mutex> lock(mapLock);
	fingerprints_->insert({ port_, res });
	
}