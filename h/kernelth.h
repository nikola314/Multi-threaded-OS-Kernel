/*
 * kernelth.h
 *
 *  Created on: Jun 18, 2018
 *      Author: OS1
 */

#ifndef H_KERNELTH_H_
#define H_KERNELTH_H_
#include "thread.h"

class KernelThread : public Thread{
public:
	static volatile int work;
	KernelThread();
};


#endif /* H_KERNELTH_H_ */
