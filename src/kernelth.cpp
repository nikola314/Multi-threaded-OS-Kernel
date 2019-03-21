
/*
 * kernelth.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: OS1
 */
#include "system.h"
#include "kernelth.h"

KernelThread::KernelThread():Thread(defaultStackSize, defaultTimeSlice) {

}
