/*
 * semaphor.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */


#include "semaphor.h"
#include "kesem.h"
#include "system.h"
#include "macros.h"
#include "syscargs.h"
#include "pcb.h"
#include <dos.h>
int Semaphore::IDG=0;

Semaphore::Semaphore(int init) {

	id=++IDG;
//	argint1 ->init
//	argint2-> id
	/*
	new KernelSem(init,id);*/
	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	//--------------------------------
	//	lockSoft;
		sysCallArgs.argInt1=init;
		sysCallArgs.argInt2=id;
		sysCallArgs.sysArgType=SysType::SE_CONS;
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

Semaphore::~Semaphore() {
//	delete myImpl; ->ovo je sad u system::restore
//	myImpl=0;
}

int Semaphore::wait(int toBlock) {
//	return myImpl->wait(toBlock);
	//argint1->id
	//argint2-> toblock
	//retval
	/*
	return System::getKSemById(id)->wait(toBlock);*/
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
		sysCallArgs.argInt2=toBlock;
		sysCallArgs.sysArgType=SysType::SE_WAIT;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
		return System::running->retval;
	//----------------------------------


}

void Semaphore::signal() {
//	myImpl->signal();
	//argint1 - > id
	/*
	System::getKSemById(id)->signal();*/
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
		sysCallArgs.sysArgType=SysType::SE_SIGNAL;
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

int Semaphore::val() const {
//	return myImpl->val();
	//argint1 id
	//retval val
	/*
	return System::getKSemById(id)->val();
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
		sysCallArgs.sysArgType=SysType::SE_VAL;
		asm push cx;
		asm push dx;
		asm mov dx, offs;
		asm mov cx, segment;
		asm int 60h;
		asm pop dx;
		asm pop cx;
	//	unlockSoft;
		return System::running->retval;
	//----------------------------------

}
