#pragma once
#include "farm.h"
#include "task.h"
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
using std::unique_lock;
using std::vector;
using std::thread;


void Farm::add_task(Task *task)
{
	unique_lock<mutex> lock(queueLock);
	taskQueue.push(task);
}

int Farm::getNumTasks() {
	unique_lock<mutex> lock(queueLock);
	return taskQueue.size();
}

void Farm::run()
{
	for (int i = 0; i < thread::hardware_concurrency(); i++) {
		threadPool.push_back(new thread([&] {
			Task* task;
			
			while (true) {
				unique_lock<mutex> lock(queueLock);
				if (taskQueue.empty()) {
					return;
				}
				task = taskQueue.front();
				taskQueue.pop();
				lock.unlock();
				task->run();
				delete task;
			}
		}));
	}
	for (auto i : threadPool) {
		i->join();
	}
}