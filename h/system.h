/*
 * system.h
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#ifndef H_SYSTEM_H_
#define H_SYSTEM_H_

typedef void interrupt (*pInterrupt)(...);
typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
typedef int ID;

class List;
class PCB;
class IVTEntry;
class Sleepy;
class EventList;
class SemList;
class KernelSem;
class KernelEv;
class SysCallArgs;

class System {

public:
		static int volatile prohibitedContextSwitch;
		static int volatile forcedTimerCall;
		static int volatile requestedContextSwitch;
		static List* pcbList;
		static PCB* idlePCB;
		static PCB* running;
		static pInterrupt oldTimer;
		static Time currentExecution;
		static volatile PCB* temp;
		static Sleepy* sleepList;
		static EventList* eventList;
		static SemList* semList;

		static SysCallArgs* sysCallArgs;
		static PCB* kernelPCB;
		static int kernelActive;

		static int isIdle;
		static int isMain;
		static int isKernel;
		static int ID;

		static void initializer();
		static void restore();
		static void interrupt timer(...);
		static void _dispatch();
		static void replaceTimer();
		static void restoreTimer();
		static PCB* getPcbById(int);
		static KernelEv* getKEvById(int);
		static KernelSem* getKSemById(int);
		static void unblockPcb(PCB*);
		static void blockRunning(List*);
		static void interrupt systemCall(...);
		static void changeRunning();

		static void interrupt returnToUser();
		static void putKernelArgument(unsigned,unsigned);
		static void kernelRun(SysCallArgs*);
		static void insertSysCall();

};

#endif /* H_SYSTEM_H_ */
