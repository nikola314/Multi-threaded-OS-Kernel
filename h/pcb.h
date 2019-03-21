/*
 * pcb.h
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#ifndef H_PCB_H_
#define H_PCB_H_

typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
typedef int ID;

class Thread;
class List;

enum State{
	NEW=0, BLOCKED, READY, IDLE=3, KERNEL, DONE
};

class PCB {
public:
	Thread* myThread;
	Time timeSlice;
	unsigned ss,sp,bp;
	unsigned* stack;
	StackSize stackSize;
	State state;
	ID id;
	List* blocked;
	int retval;

	void initStack();
	PCB(Thread*, StackSize,Time);
	void start();
	~PCB();
	void unblockWaitingThreads();
	void initKernelStack();
	void putKernelArgument(unsigned,unsigned);

	friend class System;
};

#endif /* H_PCB_H_ */
