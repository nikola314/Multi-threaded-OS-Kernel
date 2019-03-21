/*
 * syscargs.h
 *
 *  Created on: Jun 18, 2018
 *      Author: OS1
 */

#ifndef H_SYSCARGS_H_
#define H_SYSCARGS_H_

class Thread;

class SysType{
public:
	enum type{
		TH_START, TH_WTC, TH_SLEEP, TH_CONS, TH_DEST,
		SE_WAIT, SE_SIGNAL, SE_VAL, SE_CONS, SE_DEST,
		EV_WAIT, EV_SIGNAL, EV_CONS, EV_DEST,
		DISPATCH
	};
	type argType;
	SysType(type t){
		argType=t;
	}
};



class SysCallArgs{
public:
	SysType::type sysArgType;
	int retval;
	int argInt1;
	int argInt2;
	long argLong;
	char argChar;
	Thread* argth;

};



#endif /* H_SYSCARGS_H_ */
