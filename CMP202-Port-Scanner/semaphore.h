/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			semaphore.h
*  Purpose:			Semaphore definitions
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#pragma once
#ifndef SEMAPHORE
#define SEMAPHORE

#include <atomic>
#include <cstdlib>
using std::atomic;




class Semaphore
{
public:
	//Take a provided integer and add it to the atomic count_ to initialise
	Semaphore(int start = 0) {
		count_ += std::abs(start);
	}
	void signal();
	void wait();


private:
	atomic<int> count_ = 0;
};



#endif