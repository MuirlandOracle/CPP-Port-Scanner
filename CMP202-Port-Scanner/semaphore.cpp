/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			semaphore.h
*  Purpose:			Semaphore definitions
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/


#include "semaphore.h"


//On Signal, add one to the atomic<int> count_ var
void Semaphore::signal() {
	++count_;
}

//On wait, ensure that count is > 1, then subtract one and return
void Semaphore::wait() {
	while (count_ < 1) {
		continue;
	}
	--count_;
}