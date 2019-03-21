
/*
 * sleepy.cpp
 *
 *  Created on: Jun 13, 2018
 *      Author: OS1
 */

#include "macros.h"
#include "SCHEDULE.H"
#include "sleepy.h"
#include "pcb.h"

#define START_SIZE 150

Sleepy::Sleepy() {
	head=0;

	//new:

}

void Sleepy::insert(PCB* pcb, Time t) {
	/*if(pcb==0 || t<=0) return;
	lockHard;
	pcb->state=BLOCKED;
	if(head==0) head=new Elem(pcb,t);
	else {
		Time tt=t;
		Elem* cur=head, *prev=0;
		while((cur!=0) && ((tt-cur->t)>=0)){
			tt-=cur->t;
			prev=cur;
			cur=cur->next;
		}
		Elem* temp=new Elem(pcb, tt,cur);
		if(prev==0) head=temp;
		else prev->next=temp;
		temp=temp->next;
		while(temp!=0){
			temp->t=temp->t-tt;
			temp=temp->next;
		}
	}
	unlockHard;
	*/

	/*
	pcb->state=BLOCKED;
	lockHard;
	if(head==0) head=new Elem(pcb,t);
	else{
		Elem*temp=new Elem(pcb, t, head);
		head=temp;
	}
	unlockHard;
	*/

	//new:
	Time tt=t;
	//bilo <=
	if(pcb==0 || t<0) return;
	lockHard;
	pcb->state=BLOCKED;
	if(head==0) head=new Elem(pcb,t,0);
	else{
		Elem*prev=0;
		for(Elem* e=head;e!=0;e=e->next){
			if(tt>(e->t)){
				tt=tt-(e->t);
				prev=e;
			}
			else break;
		}
		if(prev==0){
			head=new Elem(pcb, tt, head);
			prev=head->next;
		}
		else{
			prev->next=new Elem(pcb, tt, prev->next);
			prev=prev->next->next;
		}
		if(prev){
			prev->t=prev->t-tt;
		}

	}
	unlockHard;
}

void Sleepy::update() {
/*	if(head==0) return;
	lockHard;
	head->t=head->t-1;
	if((--(head->t))==0){
		while((head->t)==0){
			Elem* temp=head;
			temp->info->state=READY;
			Scheduler::put(temp->info);
			head=head->next;
			delete temp;
			if(head==0) break;
		}
	}
	unlockHard;
	*/
/*
	if(head==0)return;
	lockHard;
	Elem*prev=0, *temp=head;
	while(temp!=0){
		if(--(temp->t)==0){
			if(prev==0) {
				head=temp->next;
			}
			else {
				prev->next=temp->next;
			}
			temp->info->state=READY;
			Scheduler::put(temp->info);
		}
		prev=temp;
		temp=temp->next;
	}
	unlockHard;
	*/

	if(head==0) return;
	lockHard;
	head->t-=1;
	Elem* temp=head, *prev=0;
	while(temp->t==0){
		temp->info->state=READY;
		Scheduler::put(temp->info);
		prev=temp;
		temp=temp->next;
		delete prev;
		if(temp==0) break;
	}
	head=temp;
	unlockHard;
}
