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
	std::size_t received;
	sf::TcpSocket sock;
	sock.connect(address_, port_);
	sock.send("Fingerprint\n", 13);
	sock.setBlocking(false);
	//Timeout
	timeout::time_point start = timeout::now();
	while (duration_cast<milliseconds>(timeout::now() - start).count() < 5000) {
		if (sock.receive(data, resLength, received) == sf::Socket::Done) {
			unique_lock<mutex> lock(coutLock);
			//Format the data
			cout << header;
			for (int i = 0; i < received; i++) { cout << data[i]; }
			cout << "\n";
			return;
		}
	}
	
	unique_lock<mutex> lock(coutLock);
	cout << header + "Couldn't retrieve header\n";
	
}