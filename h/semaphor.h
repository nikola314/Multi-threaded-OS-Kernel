/*
 * semaphor.h
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#ifndef H_SEMAPHOR_H_
#define H_SEMAPHOR_H_

typedef int ID;

class KernelSem;

class Semaphore {

public:
	 Semaphore (int init=1);
	 virtual ~Semaphore ();
	 virtual int wait (int toBlock);
	 virtual void signal();
	 int val () const; // Returns the current value of the semaphore
	 static int IDG;
private:
	 ID id;
// 	 KernelSem* myImpl;

};



#endif /* H_SEMAPHOR_H_ */
