/*
 * main.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#include "system.h"
#include "thread.h"
#include <iostream.h>
#include "macros.h"
#include <stdlib.h>
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lockHard;
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlockHard;
	return res;
}


extern int userMain(int argc,char *argv[]);

class UserThread:public Thread{
	int argc;
	char** argv;
	int retval;
	friend int main(int,char*[]);
public:
	UserThread(int a, char** ar) : Thread(3*defaultStackSize, defaultTimeSlice ){
		argc=a;
		argv=ar;
		retval=0;
	}

	void run(){
		retval=userMain(argc,argv);
	}

};

extern int syncPrintf(const char *format, ...);

int main(int argc, char* argv[]){
	System::initializer();
	int i;
	UserThread* user=new UserThread(argc,argv);
	user->start();
	user->waitToComplete();
	int retval=user->retval;
	delete user;
	System::restore();
	return retval;
}



