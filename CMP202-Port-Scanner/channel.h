/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			channel.h
*  Purpose:			Channel structure implementation
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#pragma once
#ifndef CHANNEL
#define CHANNEL


#include <mutex>
#include <list>
#include "semaphore.h"

using std::unique_lock;
using std::list;
using std::mutex;

//Templated channel for signalling between threads
template <typename T>
class channel {
public:
	//Write into the channel, taking a unique_lock on the list
	void write(T data) {
		unique_lock<mutex> lock(buffMutex);
		buff.push_back(data);
		//Signal the semaphore to let anything waiting for a read to accept the input
		sem.signal();
	}

	T read() {
		//Wait until there is something in the buffer
		sem.wait();
		//Get a unique_lock on the list and remove the item from the buffer
		unique_lock<mutex> lock(buffMutex);
		T item = buff.front();
		buff.pop_front();
		//Return the items
		return item;
	}

private:
	//List to contain the buffer for reading and writing
	list<T> buff;
	//Initialising a semaphore (default value 0)
	Semaphore sem;
	//Mutex to protect the buffer
	mutex buffMutex;
};

#endif