/*
 * event.h
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#ifndef H_EVENT_H_
#define H_EVENT_H_

typedef unsigned char IVTNo;
typedef int ID;

class KernelEv;

class Event {

public:
	 Event (IVTNo ivtNo);
	 ~Event ();
	 void wait ();
	 static int IDG;
protected:

	 friend class KernelEv;
	 void signal(); // can call KernelEv

private:
	 ID id;
//	 KernelEv* myImpl;

};



#endif /* H_EVENT_H_ */
