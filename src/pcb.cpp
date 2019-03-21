/*
 * pcb.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#include "pcb.h"
#include "thread.h"
#include "system.h"
#include "list.h"
#include "SCHEDULE.H"
#include <dos.h>
#include "macros.h"

void PCB::initStack(){
	if(state==NEW || state==IDLE){
			lockHard;
			stack=new unsigned[stackSize];
			unlockHard;
	#ifndef BCC_BLOCK_IGNORE
			stack[stackSize-1]=FP_SEG(myThread);
			stack[stackSize-2]=FP_OFF(myThread);
			stack[stackSize-5]=0x200;
			stack[stackSize-6]=FP_SEG(&Thread::wrapper);
			stack[stackSize-7]=FP_OFF(&Thread::wrapper);
			ss=FP_SEG(stack+stackSize-16);
			sp=FP_OFF(stack+stackSize-16);
			bp=sp;
	#endif
		if(state==NEW) state=READY;
	}
}

PCB::PCB(Thread* t, StackSize ss,Time ts){
	myThread=t;
	stackSize= ss>65536?65536:ss;
	stackSize/=sizeof(unsigned);
	timeSlice=ts>=0?ts:defaultTimeSlice;
	this->ss=sp=bp=0;
	stack=0;
	if(t!=0)id=t->id;
	else id=++(System::ID);
	state=NEW;
	blocked = new List();
	if(System::isIdle) {
		state=IDLE;
		System::idlePCB=this;
	}
	if(System::isMain) {
		state=READY;
	}
	if(System::isKernel){
		state=KERNEL;
		stack=0;
		System::kernelPCB=this;
	}
	System::pcbList->insert(this);
}

void PCB::start(){
//	if(state==KERNEL) initKernelStack();
	if(state!=NEW && state!=IDLE) return;
	initStack();
	if(state!=IDLE) {
		state=READY;
		Scheduler::put(this);
	}
}

PCB::~PCB(){
	lockHard;
	delete [] stack;
	delete blocked;
	stack=0;
	blocked=0;
	unlockHard;
}

void PCB::unblockWaitingThreads(){
	PCB* temp=0;
	while((temp=blocked->getAndRemove())!=0){
		temp->state=READY;
		Scheduler::put(temp);
	}
}

void PCB::putKernelArgument(unsigned segment, unsigned offs){
//	if(this!=System::kernelPCB) return;
	lockHard;
	if(stack!=0) delete[]stack;
	stack=new unsigned[stackSize];
	unlockHard;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize-1]=segment;
	stack[stackSize-2]=offs;
	stack[stackSize-3]=0;
	stack[stackSize-4]=0;
	stack[stackSize-5]=0x200;
	stack[stackSize-6]=FP_SEG(&System::kernelRun);
	stack[stackSize-7]=FP_OFF(&System::kernelRun);
	ss=FP_SEG(stack+stackSize-16);
	sp=FP_OFF(stack+stackSize-16);
	bp=sp;
#endif
}



