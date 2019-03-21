/*
 * event.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#include "event.h"
#include "keevent.h"
#include "system.h"
#include "macros.h"
#include "syscargs.h"
#include <dos.h>
int Event::IDG=0;

Event::Event(IVTNo ivtNo) {

	id=++IDG;
	SysCallArgs sysCallArgs;
	unsigned segment;
	unsigned offs;
#ifndef BCC_BLOCK_IGNORE
	segment=FP_SEG(&sysCallArgs);
	offs=FP_OFF(&sysCallArgs);
#endif
	/*
//	myImpl=
	//argint1 id
	//argint2 ivtno
	new KernelEv(ivtNo,id);
*/

	//--------------------------------
	//	lockSoft;
		sysCallArgs.argInt1=id;
		sysCallArgs.argInt2=ivtNo;
		sysCallArgs.sysArgType=SysType::EV_CONS;
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

Event::~Event() {
//	delete myImpl; ->kernelevent sada u listi pa brisati na kraju
//	myImpl=0;
}

void Event::wait() {
//	myImpl->wait();
	//intarg1 id
	/*
	System::getKEvById(id)->wait();
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
		sysCallArgs.sysArgType=SysType::EV_WAIT;

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

void Event::signal() {
//	myImpl->signal();
	//intartg1 id
	/*
	System::getKEvById(id)->signal();
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
		sysCallArgs.sysArgType=SysType::EV_SIGNAL;
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
