/*
 * prepent.h
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */

#ifndef H_PREPENT_H_
#define H_PREPENT_H_

#include "ivtentry.h"
#include "system.h"
#include "macros.h"
/*
#define PREPAREENTRY(entryNum,callOld);\
		void interrupt intNum##entryNum(...);\
		IVTEntry ivtNum##entryNum(entryNum,intNum##entryNum);\
		void interrupt intNum##entryNum(...){\
			(ivtNum##entryNum).signalMyEvent();\
			if(callOld!=0){\
				(ivtNum##entryNum).callOldRoutine();\
			}\
		}	//		if(System::kernelActive==0)\
	//			dispatch();\
	//	}
	//
*/
#define PREPAREENTRY(entryNum,callOld);\
		void interrupt intNum##entryNum(...);\
		IVTEntry ivtNum##entryNum(entryNum,intNum##entryNum);\
		void interrupt intNum##entryNum(...){\
			(ivtNum##entryNum).signalMyEvent();\
			if(callOld!=0){\
				(ivtNum##entryNum).callOldRoutine();\
			}\
			if(System::kernelActive==0){\
				lockHard;\
				dispatch();\
				unlockHard;}\
		}




#endif /* H_PREPENT_H_ */
