/* Project:			C++ Multi-Threaded Port Scanner
*  Title:			task.h
*  Purpose:			Abstract class definition for a Task. Referred to by the farm when starting threads
*  Author:			AG
*  Last Updated:	06/05/21 (AG)
*/

#ifndef TASK_H
#define TASK_H

class Task
{
public:
	//Virtual deconstructor to be overwritten if necessary
	virtual ~Task()
	{
	}

	//Virtual run() to be overwritten by sub-class tasks (ScanPort and FingerprintPort)
	virtual void run() = 0;
};

#endif
