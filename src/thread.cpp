/*
 * thread.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "system.h"
#include "list.h"
#include "macros.h"
#include <iostream.h>
#include "sleepy.h"
#include "syscargs.h"
#include <dos.h>

void Thread::start(){
	/*
#ifndef BCC_BLOCK_IGNORE
	lockHard;
#endif
	//staviti ID u arg1
	PCB* me=System::getPcbById(id);
	me->start();
#ifndef BCC_BLOCK_IGNORE
	unlockHard;
#endif
*/
	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------
	//	lockSoft;
		sysCallArgs.argInt1=id;
		sysCallArgs.sysArgType=SysType::TH_START;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
	//----------------------------------

}

extern int syncPrintf(const char *format, ...);

void Thread::waitToComplete(){
//	syncPrintf("\nwaitToComplete %d -> %d\n",System::running->id,id);
	/*
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	//argint1 = this.id
	PCB* me=System::getPcbById(id);
	if(System::running==System::idlePCB) {unlockHard; return;}
	if(me==0) {unlockHard; return; }
	if((me->state==READY || me->state==BLOCKED) && System::running!=me){
		me->blocked->insert(System::running);
		System::running->state=BLOCKED;
		unlockHard;
		dispatch();
	}
	else unlockHard;
#endif
*/
	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------

	//	lockSoft;
		sysCallArgs.argInt1=id;
		sysCallArgs.sysArgType=SysType::TH_WTC;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
	//----------------------------------


}

Thread::~Thread(){
	/*
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	PCB* myPCB=System::getPcbById(id);
	if(myPCB==0) {unlockHard; return;}
	unlockHard;
	waitToComplete();
//	lockHard;
//	delete myPCB; -> ovo sad u system::restore -->list vise ne treba metoda remove by id
//	unlockHard;
#endif
*/
	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------
	//	lockSoft;
		sysCallArgs.argInt1=id;
		sysCallArgs.sysArgType=SysType::TH_WTC;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
	//----------------------------------

}

void Thread::sleep(Time timeToSleep){
	/*
	//intarg1-> timetosleep
	if(timeToSleep==0) return;
	System::sleepList->insert(System::running,timeToSleep);
	dispatch();
*/
	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------
	//	lockSoft;
		sysCallArgs.argInt1=timeToSleep;
		sysCallArgs.sysArgType=SysType::TH_SLEEP;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
	//----------------------------------


}

Thread::Thread (StackSize stackSize, Time timeSlice){
	/*
#ifndef BCC_BLOCK_IGNORE
	lockHard;
#endif
	//argint1->id ne treba
	//argth-> this
	//arglong ->stackSize
	//argint2 ->timeslice
	id=++(System::ID); //ovo ostaviti
	new PCB(this, stackSize,timeSlice);
#ifndef BCC_BLOCK_IGNORE
	unlockHard;
#endif
*/

	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------
		id=++(System::ID);
	//	lockSoft;
		sysCallArgs.argInt2=timeSlice;
		sysCallArgs.argLong=stackSize;
		sysCallArgs.argth=this;
		sysCallArgs.sysArgType=SysType::TH_CONS;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
	//----------------------------------
}

void Thread::wrapper(Thread* running){
	running->run();
//	cout<<"Thread::wrapper -> posle run()";
	lockSoft;
	PCB* me= System::getPcbById(running->id);
	if(me==0) return;
	me->state=DONE;
	me->unblockWaitingThreads();
	unlockSoft;
	dispatch();
}

void dispatch(){

#ifndef BCC_BLOCK_IGNORE
//	lockHard;
#endif
//	System::_dispatch();
#ifndef BCC_BLOCK_IGNORE
//	unlockHard;
#endif

	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------
	//	lockSoft;
		sysCallArgs.sysArgType=SysType::DISPATCH;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;

}
