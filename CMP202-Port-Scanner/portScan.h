#pragma once
#include "task.h"
#include <mutex>
#include <atomic>
#include <set>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#define TIMEOUT_MILLISECS 100;

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
	mutex openPortsMutex_;
	set<int>* openPorts_;
	sf::Time timeout_ = sf::milliseconds(100);
	atomic<int>& openPortsNum_;
	atomic<int>& closedPortsNum_;
};


class FingerprintPort : public Task
{
public:
	FingerprintPort(const sf::IpAddress& address, const int port)
		: address_(address), port_(port)
	{}

	void run();
private:
	const sf::IpAddress address_;
	const int port_;
	mutex coutLock;
};