/*
 * ivtentry.h
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry{

	pInterrupt oldRoutine;
	pInterrupt newRoutine;
	KernelEv* myEvent;
	IVTNo ivtNo;

public:
	static IVTEntry* ivtEntries[256];
	IVTEntry(IVTNo, pInterrupt);
	~IVTEntry();
	void setEvent(KernelEv*);
	void signalMyEvent();
	void callOldRoutine();
	void resetEvent();

};



#endif /* H_IVTENTRY_H_ */
