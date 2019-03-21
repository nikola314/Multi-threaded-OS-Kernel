/*
 * keevent.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#include "keevent.h"
#include "system.h"
#include "macros.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "list.h"
#include "prepent.h"
#include "thread.h"
IVTEntry* IVTEntry::ivtEntries[256]={0};

extern int syncPrintf(const char *format, ...);

KernelEv::KernelEv(IVTNo ivtNo, ID ii) {
//	lockHard;
	this->ivtNo=ivtNo;
	pcbBlocked=0;
	id=ii;
	owner=System::running;
	value=0;
	IVTEntry::ivtEntries[ivtNo]->setEvent(this);
	System::eventList->insert(this);
//	syncPrintf("\nOwner: %p",owner);
//	unlockHard;
}

KernelEv::~KernelEv() {
//	if(IVTEntry::ivtEntries[ivtNo]!=0)	IVTEntry::ivtEntries[ivtNo]->resetEvent();
}

void KernelEv::wait() {
//	syncPrintf("\nWaiting: %p",System::running);
	if(System::running!=owner) return;
	lockHard;
	if(value==0){
		if(pcbBlocked==0){
			System::running->state=BLOCKED;
			unlockHard;
			pcbBlocked=1;
			dispatch();
		}
		else unlockHard;
	}
	else {value=0;unlockHard;}

}

void KernelEv::signal() {
	lockHard;
/*	if(pcbBlocked==0) value=1;
	else{
		owner->state=READY;
		Scheduler::put(owner);
		pcbBlocked=0;
	}
	*/
	if(value<0){
		owner->state=READY;
		Scheduler::put(owner);
		pcbBlocked=0;
		value++;
	}
	else value=1;
	unlockHard;
}
