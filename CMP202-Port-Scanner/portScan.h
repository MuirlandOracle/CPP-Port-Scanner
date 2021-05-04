#pragma once
#include "task.h"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

class ScanPort : public Task
{
public:
	ScanPort(const sf::IpAddress& address, const int port)
		: address_(address), port_(port)
	{}

	void run();
private:
	const sf::IpAddress address_;
	const int port_;
	sf::Time timeout_ = sf::milliseconds(100);
};