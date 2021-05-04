#pragma once
#include "task.h"
#include <mutex>
#include <atomic>
#include <set>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

using std::mutex;
using std::set;
using std::atomic;


class ScanPort : public Task
{
public:
	ScanPort(const sf::IpAddress& address, const int port, set<int>* openPorts, atomic<int>& openPortsNum, atomic<int>& closedPortsNum)
		: address_(address), port_(port), openPorts_(openPorts), openPortsNum_(openPortsNum), closedPortsNum_(closedPortsNum)
	{}

	void run();
private:
	const sf::IpAddress address_;
	const int port_;
	sf::Time timeout_ = sf::milliseconds(100);
	mutex openPortsMutex_;
	set<int>* openPorts_;
	atomic<int>& openPortsNum_;
	atomic<int>& closedPortsNum_;
};