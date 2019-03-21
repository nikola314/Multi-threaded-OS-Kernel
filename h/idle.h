/*
 * idle.h
 *
 *  Created on: Jun 4, 2018
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_

#include "thread.h"
extern int syncPrintf(const char *format, ...);
class Idle:public Thread{
public:
	Idle():Thread(defaultStackSize, 1){doWork=1;}
	static int volatile doWork;
	void run(){
		while(doWork!=0){
		//	dispatch();
		}
	}
};



#endif /* H_IDLE_H_ */
