/*
 * macros.h
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#ifndef H_MACROS_H_
#define H_MACROS_H_


#define lockSoft System::prohibitedContextSwitch=1;
#define unlockSoft System::prohibitedContextSwitch=0;


#define lockHard\
	asm pushf;\
	asm cli;


#define unlockHard asm popf;


#define saveContext\
			asm mov tsp, sp;\
			asm	mov tss, ss;\
			asm	mov tbp, bp;



#define restoreContext\
			asm	mov sp,tsp;\
			asm	mov ss,tss;\
			asm	mov bp,tbp;




#endif /* H_MACROS_H_ */
