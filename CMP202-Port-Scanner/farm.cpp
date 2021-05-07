/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			farm.cpp
*  Purpose:			Implementation of the Farm class. Handles threading
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#pragma once
#include "farm.h"
#include "task.h"
#include <thread>
#include <mutex>
using std::unique_lock;
using std::thread;

//Adds a task to the Farm task queue
void Farm::add_task(Task *task)
{
	//Adding a unique_lock in case any tasks involve adding further tasks to the list
	unique_lock<mutex> lock(queueLock);
	taskQueue.push(task);
}


//Starts the entire farm
void Farm::run()
{
	//Creates a number of threads equal the the number of cores/logical processors supported by the hardware
	for (int i = 0; i < thread::hardware_concurrency(); i++) {
		threadPool.push_back(new thread([&] {
			//Creates a (null) task pointer to store a reference to the task being worked on
			Task* task;
			//Loop until the queue is empty, then exit
			while (true) {
				//Lock the queue whilst checking if it's empty and removing a task if not
				unique_lock<mutex> lock(queueLock);
				if (taskQueue.empty()) {
					return;
				}
				task = taskQueue.front();
				taskQueue.pop();
				lock.unlock();
				//Run the task, then delete it so it's not cluttering up memory
				task->run();
				delete task;
			}
		}));
	}
	//Wait for all the threads to complete before exiting
	for (auto i : threadPool) {
		i->join();
	}
	//Clear the vector storing the threads so that the farm can be reused
	threadPool.clear();
}