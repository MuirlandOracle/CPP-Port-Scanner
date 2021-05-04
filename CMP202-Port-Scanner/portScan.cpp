#pragma once
#include "task.h"
#include "portScan.h"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>

void ScanPort::run() {
	bool res = sf::TcpSocket().connect(address_, port_, timeout_) == sf::Socket::Done;
	if (res) {
		std::cout << "Port " << port_ << " is open\n";
	}
	else {
		std::cout << "Port " << port_ << " is closed\n";
	}
}