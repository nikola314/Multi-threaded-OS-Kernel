/*
 * sleepy.h
 *
 *  Created on: Jun 13, 2018
 *      Author: OS1
 */

#ifndef H_SLEEPY_H_
#define H_SLEEPY_H_

typedef unsigned int Time;

class PCB;

class Sleepy{

	struct Elem{
		Elem* next;
		PCB* info;
		Time t;
		Elem(PCB* i, Time tt,Elem* n=0){
			info=i;
			next=n;
			t=tt;
		}
	};
	Elem* head;


public:
	Sleepy();
	void insert(PCB* pcb,Time t);
	void update();

};


#endif /* H_SLEEPY_H_ */
