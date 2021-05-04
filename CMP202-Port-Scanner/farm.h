#ifndef FARM_H
#define FARM_H

#include "task.h"
#include <queue>
#include <mutex>

using std::queue;
using std::mutex;
using std::vector;
using std::thread;

// FIXME - You will need to add #includes here (probably <mutex> at least)

/** A collection of tasks that should be performed in parallel. */
class Farm {
public:
	// DO NOT CHANGE the public interface of this class.
	// You only need to implement the existing methods.

	/** Add a task to the farm.
	    The task will be deleted once it has been run. */
	void add_task(Task *task);

	/** Run all the tasks in the farm.
	    This method only returns once all the tasks in the farm
		have been completed. */
	void run();

	int getNumTasks();

private:
	std::mutex queueLock;
	vector <thread*> threadPool;
	std::queue<Task *> taskQueue;
};

#endif
