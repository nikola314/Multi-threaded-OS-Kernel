/*
 * keevent.h
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#ifndef H_KEEVENT_H_
#define H_KEEVENT_H_

typedef unsigned char IVTNo;
typedef int ID;

class PCB;

class KernelEv {

public:
	 KernelEv (IVTNo ivtNo, ID id);
	 ~KernelEv ();
	 void wait ();
	 void signal();
	 ID id;
private:
	 PCB* owner;
	 IVTNo ivtNo;
	 int value;
	 int pcbBlocked;
protected:
	 friend class System;
};



#endif /* H_KEEVENT_H_ */
