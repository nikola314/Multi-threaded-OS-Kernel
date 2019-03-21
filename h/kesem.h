/*
 * kernelsem.h
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#ifndef H_KESEM_H_
#define H_KESEM_H_
typedef int ID;


class List;

class KernelSem {
public:
	KernelSem (int init=1,ID id);
	~KernelSem ();
	int wait (int toBlock);
	void signal();
	int val () const;
	ID id;
private:
	volatile int value;
	List* blocked;
protected:
	friend class System;
};

#endif /* H_KESEM_H_ */
