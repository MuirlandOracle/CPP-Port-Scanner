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

void ScanPort::run() {
	bool res = sf::TcpSocket().connect(address_, port_, timeout_) == sf::Socket::Done;
	if (res) {
		unique_lock<mutex> lock(openPortsMutex_);
		openPorts_->insert(port_);
		++openPortsNum_;
	}
	else {
		++closedPortsNum_;
	}
}


void FingerprintPort::run() {
	const int resLength = 300;
	char data[resLength];
	string header = "===== " + address_.toString() + ":" + to_string(port_) + " =====\n";
	string res = "";
	std::size_t received;
	sf::TcpSocket sock;
	sock.connect(address_, port_);
	sock.send("Fingerprint\n", 13);
	sock.setBlocking(false);
	//Timeout
	timeout::time_point start = timeout::now();
	do {
		if (sock.receive(data, resLength, received) == sf::Socket::Done) {
			unique_lock<mutex> lock(mapLock);
			//Format the data
			for (int i = 0; i < received; i++) { res += data[i]; }
			break;
		}
	} while (duration_cast<milliseconds>(timeout::now() - start).count() < 5000);
	if (res == "") {
		res = "Couldn't retrieve header\n";
	}
	res = header + res;
	unique_lock<mutex> lock(mapLock);
	fingerprints_->insert({ port_, res });
	
}