#pragma once
#include "task.h"
#include "portScan.h"
#include <mutex>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

using std::unique_lock;
using std::mutex;

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