#ifndef FARM_H
#define FARM_H

#include "task.h"
#include <queue>
#include <mutex>

using std::queue;
using std::mutex;
using std::vector;
using std::thread;


class Farm {
public:

	void add_task(Task *task);


	void run();

	int getNumTasks();

private:
	std::mutex queueLock;
	vector <thread*> threadPool;
	std::queue<Task *> taskQueue;
};

#endif
