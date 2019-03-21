/*
 * thread.h
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#ifndef H_THREAD_H_
#define H_THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();
	static void sleep(Time timeToSleep);
	ID getId(){return id;}

protected:
	 friend class PCB;
	 Thread (StackSize stackSize = defaultStackSize, Time timeSlice =defaultTimeSlice);
	 virtual void run() {}
	 ID id; //for test

private:
//	 ID id;
	 static void wrapper(Thread* running);
};

void dispatch ();



#endif /* H_THREAD_H_ */
