/*
 * kernelsem.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#include "kesem.h"
#include "list.h"
#include "macros.h"
#include "system.h"

KernelSem::KernelSem(int i, ID ii){
	lockHard;
	blocked=new List();
	unlockHard;
	value=i;
	id=ii;
	System::semList->insert(this);

}

KernelSem::~KernelSem() {
	lockHard;
	delete blocked;
	blocked=0;
	unlockHard;
}

int KernelSem::wait(int toBlock) {
	lockHard;
	int retval;
	if(toBlock==0 && value<=0){
		retval=-1;
		unlockHard;
	}
	else{
		value--;
		if(value<0){
			retval=1;
			unlockHard;
			System::blockRunning(blocked);
		}
		else {
			retval=0;
			unlockHard;
		}
	}
	return retval;
}

void KernelSem::signal() {

	if(value<0){
		PCB* temp=blocked->getAndRemove();
		if(temp!=0){
			System::unblockPcb(temp);
		}
	}
	lockHard;
	value++;
	unlockHard;
}

int KernelSem::val() const {
	return value;
}
