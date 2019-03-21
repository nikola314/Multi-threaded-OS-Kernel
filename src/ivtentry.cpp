/*
 * ivtentry.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#include "ivtentry.h"
#include "keevent.h"
#include "macros.h"
#include <dos.h>
#include "system.h"


IVTEntry::IVTEntry(IVTNo num, pInterrupt newRout) {
	lockHard;
	ivtNo=num;
	newRoutine=newRout;
	myEvent=0;
	//oldRoutine=0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine=getvect(num);
	setvect(num, newRoutine);
#endif
	ivtEntries[num]=this;
	unlockHard;
}

void IVTEntry::signalMyEvent() {
//	lockHard;
	if(myEvent!=0) myEvent->signal();
//	unlockHard;
}


IVTEntry::~IVTEntry() {
	lockHard;
//	(*oldRoutine)();
	ivtEntries[ivtNo]=0;

#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
#endif
	oldRoutine=0;
	unlockHard;
}


void IVTEntry::setEvent(KernelEv* event) {
	lockHard;
	myEvent=event;
	unlockHard;
}


void IVTEntry::callOldRoutine() {
	lockHard;
	if(oldRoutine!=0) (*oldRoutine)();
	unlockHard;
}

void IVTEntry::resetEvent(){
	lockHard;
	myEvent=0;
	unlockHard;
}




