/*
 * system.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#include "system.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "kernelth.h"
#include "list.h"
#include <dos.h>
#include "macros.h"
#include "idle.h"
#include "prepent.h"
#include <iostream.h>//del
#include <stdio.h>//del
#include "sleepy.h"
#include "syscargs.h"
#include "keevent.h"
#include "kesem.h"
extern void tick();

volatile unsigned tsp, tss,tbp;
int volatile System::requestedContextSwitch=0;
int volatile System::prohibitedContextSwitch=0;
int volatile System::forcedTimerCall=0;
List* System::pcbList=0;
PCB* System::idlePCB=0;
PCB* System::running=0;
pInterrupt System::oldTimer=0;
Time System::currentExecution=0;
volatile PCB* System::temp=0;
int System::isMain=0;
int System::isIdle=0;
int System::ID=0;
Sleepy* System::sleepList=0;
EventList* System::eventList=0;
SemList* System::semList=0;
int volatile Idle::doWork=0;
SysCallArgs* System::sysCallArgs=0;
int System::isKernel=0;
PCB* System::kernelPCB=0;
int volatile KernelThread::work=0;
int System::kernelActive=0;

void System::initializer() {
#ifndef BCC_BLOCK_IGNORE
	lockHard;
#endif
	sleepList=new Sleepy();
	pcbList=new List();
	semList=new SemList();
	eventList=new EventList();
	isMain=1;
	running=new PCB(0, defaultStackSize, defaultTimeSlice);
	isMain=0;
	kernelActive=0;

	insertSysCall();
	sysCallArgs=new SysCallArgs();

	isKernel=1;
	new PCB(0,defaultStackSize,defaultTimeSlice);
	isKernel=0;
	kernelPCB->start();
//	KernelThread::work=1;//ne treba sad

	isIdle=1;
	Thread* idl=new Idle();
	idl->start();
	isIdle=0;

	replaceTimer();
#ifndef BCC_BLOCK_IGNORE
	unlockHard;
#endif
}

void System::restore() {
	//deletovanje listi i pcbova iz njih
	//restauracija prekidnih rutina
	restoreTimer();
	lockHard;
	delete sleepList;
	pcbList->dealocatePcbs();
	delete pcbList;
	delete semList;
	delete eventList;
	sleepList=0;
	pcbList=0;
	semList=0;
	eventList=0;
	unlockHard;
}

void System::_dispatch() {
	forcedTimerCall=1;
	timer();
}

extern int syncPrintf(const char *format, ...);



void interrupt System::timer(...){
	if(!forcedTimerCall){
		tick();
		(*oldTimer)();
		System::currentExecution++;
		sleepList->update();
	}
	if(forcedTimerCall || (!forcedTimerCall && currentExecution>=running->timeSlice && running->timeSlice!=0)){
		if(prohibitedContextSwitch) {requestedContextSwitch=1; return;}
	//	if(requestedContextSwitch==1) requestedContextSwitch=0;
		forcedTimerCall=0;
		#ifndef BCC_BLOCK_IGNORE
		//	saveContext;
		asm mov tsp, sp;
		asm	mov tss, ss;
		asm	mov tbp, bp;
		#endif

		running->sp=tsp;
		running->ss=tss;
		running->bp=tbp;

		System::changeRunning();

		tss=running->ss;
		tsp=running->sp;
		tbp=running->bp;

		#ifndef BCC_BLOCK_IGNORE
			//	restoreContext;
			asm	mov sp,tsp;
			asm	mov ss,tss;
			asm	mov bp,tbp;
		#endif
	}
}

void System::changeRunning(){
	lockHard;
	if(running->state==READY){
		Scheduler::put(running);
	}
	temp=Scheduler::get();
	if(temp==0){
		running=idlePCB;
	}
	else running=(PCB*)temp;
	currentExecution = 0;
	unlockHard;
}

void System::replaceTimer() {
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	oldTimer=getvect(0x08);
	setvect(0x08, System::timer);
	unlockHard;
#endif
}

void System::restoreTimer() {
#ifndef BCC_BLOCK_IGNORE
	lockHard;
	setvect(0x08,oldTimer);
	unlockHard;
#endif
}

PCB* System::getPcbById(int i) {
	return (*pcbList)[i];
}

KernelEv* System::getKEvById(int i){
	return (*eventList)[i];
}


KernelSem* System::getKSemById(int i){
	return (*semList)[i];
}

void System::unblockPcb(PCB* pcb){
	if(pcb->state==BLOCKED) {
		pcb->state=READY;
		Scheduler::put(pcb);
	}
}

void System::blockRunning(List* lst){
	if(running->state==IDLE) return;
	running->state=BLOCKED;
	lst->insert(running);
	_dispatch();
}

unsigned argSeg, argOff;

void interrupt System::systemCall(...) {
#ifndef BCC_BLOCK_IGNORE
		//	saveContext;
		asm mov tsp, sp;
		asm	mov tss, ss;
		asm	mov tbp, bp;
		asm push ax;
		asm mov ax, word ptr[bp+10];
		asm mov argOff, ax;
		asm mov ax, word ptr[bp+12];
		asm mov argSeg,ax;
		asm pop ax;
#endif
		kernelActive=1;

		running->sp=tsp;
		running->ss=tss;
		running->bp=tbp;

		//umesto init kernel stack zvati metodu koja na stack stavi argument zadat parametrom
	//	System::kernelPCB->initKernelStack();
		System::kernelPCB->putKernelArgument(argSeg,argOff);

		tsp=kernelPCB->sp;
		tss=kernelPCB->ss;
		tbp=kernelPCB->bp;

#ifndef BCC_BLOCK_IGNORE
		//	start Kernel;
		asm mov sp, tsp;
		asm	mov ss, tss;
		asm	mov bp, tbp;
#endif
}

void System::insertSysCall(){
#ifndef BCC_BLOCK_IGNORE
	setvect(0x60, System::systemCall);
#endif
}


void System::kernelRun(SysCallArgs* args){

	lockSoft;
	PCB* me=0;
	SysType::type cs=args->sysArgType;

	switch(cs){

	case SysType::TH_START:
	//		syncPrintf("TH_START ");
			me=System::getPcbById(args->argInt1);
			if(me!=0) me->start(); break;

	case SysType::TH_DEST:break;

	case SysType::TH_WTC:
	//	syncPrintf("TH_WTC ");
		me=getPcbById(args->argInt1);
		if(me == 0) break;
		if(running!=idlePCB && me!=0 && (me->state==READY || me->state==BLOCKED) && running!=me){
			running->state=BLOCKED;
			me->blocked->insert(running);

			//--
			changeRunning();
			if(requestedContextSwitch) requestedContextSwitch=0;
			//--
		}	break;

	case SysType::TH_SLEEP:
	//	syncPrintf("TH SLEEP ");
		running->retval=0;
		if(args->argInt1>0){
			sleepList->insert(running, args->argInt1);
			running->retval=1;
			changeRunning();
			if(requestedContextSwitch) requestedContextSwitch=0;
		}	break;

	case SysType::TH_CONS:
	//	syncPrintf("TH_CONST ");
		lockHard;
		new PCB(args->argth,args->argLong,args->argInt2);
		unlockHard;
		break;

	case SysType::SE_WAIT:
	//	syncPrintf("SE_WAIT  ");
		int id=args->argInt1;
		int toBlock=args->argInt2;
		KernelSem* ks=getKSemById(id);
		if(toBlock==0 && ks->value<=0){
			running->retval=-1;
		}
		else{
			lockHard;
			ks->value=ks->value-1;
			unlockHard;
			if(ks->value<0){
				running->retval=1;
				running->state=BLOCKED;
				ks->blocked->insert(running);
				changeRunning();
				if(requestedContextSwitch) requestedContextSwitch=0;

			} else running->retval=0;
		}	break;

	case SysType::SE_SIGNAL:
	//	syncPrintf("SE_SIGNAL");
		getKSemById(args->argInt1)->signal();
		break;

	case SysType::SE_VAL:
	//	syncPrintf("SE VAL ");
		running->retval=getKSemById(args->argInt1)->val();
		break;

	case SysType::SE_CONS:
	//	syncPrintf("SE CONS ");
		lockHard;
		new KernelSem(args->argInt1, args->argInt2);
		unlockHard;
		break;

	case SysType::SE_DEST:
		//nema
		break;

	case SysType::EV_WAIT:
	//	syncPrintf("EV_WAIT ");
		//razmisliti wait
		KernelEv* ke=getKEvById(args->argInt1);
		if(running==ke->owner){
			ke->value--;
			if(ke->value<0){//bilo ==
				running->state=BLOCKED;
				lockHard;
				ke->pcbBlocked=1;
				unlockHard;
				changeRunning();
				if(requestedContextSwitch) requestedContextSwitch=0;
			}
			else {
				lockHard;
				ke->value=0;
				unlockHard;
			}
		}
		break;

	case SysType::EV_SIGNAL:
	//	syncPrintf("EV_SIGNAL");
		getKEvById(args->argInt1)->signal();
		break;

	case SysType::EV_CONS:
	//	syncPrintf("EV_CONS ");
		lockHard;
		new KernelEv(args->argInt2, args->argInt1);
		unlockHard;
		break;

	case SysType::EV_DEST:
		//empty
		break;

	case SysType::DISPATCH:
		changeRunning();
		if(requestedContextSwitch) requestedContextSwitch=0;
		break;
	}
	if(System::requestedContextSwitch==1) changeRunning();

	asm cli;
	kernelActive=0;
	unlockSoft;
	requestedContextSwitch=0;
	returnToUser();
}


void interrupt System::returnToUser(){
		asm cli;
		tss=running->ss;
		tsp=running->sp;
		tbp=running->bp;
#ifndef BCC_BLOCK_IGNORE
		asm{
			mov ss, tss
			mov sp, tsp
			mov bp, tbp
		}
#endif
}








