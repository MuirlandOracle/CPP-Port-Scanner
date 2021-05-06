/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			farm.h
*  Purpose:			Class definition for the farm class
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#ifndef FARM_H
#define FARM_H

#include "task.h"
#include <queue>
#include <mutex>

using std::queue;
using std::mutex;
using std::vector;
using std::thread;

//Class definition for the Farm class -- used to organise tasks and split them between threads
class Farm {
public:

	void add_task(Task *task);
	void run();

private:
	//Mutex to protect the taskQueue
	std::mutex queueLock;
	//Queue to contain the tasks. FIFO structure
	std::queue<Task*> taskQueue;
	//Vector containing pointers to the available threads
	vector <thread*> threadPool;
	
};

#endif
