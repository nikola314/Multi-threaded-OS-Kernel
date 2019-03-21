/*
 * list.cpp
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#include "list.h"
#include "pcb.h"
#include "keevent.h"
#include "kesem.h"
#include "macros.h"

#define START_SIZE 150

Elem::Elem(PCB* i, Elem* n) {
	info=i;
	next=n;
}

PCB* List::operator [](int id) {
/*
	Elem* temp=head;
	while(temp!=0){
		if(temp->info->id==id) return temp->info;
		temp=temp->next;
	}
	return 0;
*/

	//new:
	if(id<size) return pcbVect[id];
	return 0;

}

void List::insert(PCB* pcb) {
/*
	if(head==0)	head=new Elem(pcb,0);
	else{
		Elem* temp=new Elem(pcb, head);
		head=temp;
	}
*/
	//new:
	if(pcb->id>=size) {
		lockHard;
		PCB** newVect=new PCB*[2*size];
		for(int i=0;i<size*2;i++){
			if(i<size) newVect[i]=pcbVect[i];
			else newVect[i]=0;
		}
		size*=2;
		delete[] pcbVect;
		pcbVect=newVect;
		unlockHard;
	}
	pcbVect[pcb->id]=pcb;
}

List::List() {
	head=0;

	//new:
	lockHard;
	pcbVect=new PCB*[START_SIZE];
	unlockHard;
	size=START_SIZE;
	for(int i=0;i<START_SIZE;i++){
		pcbVect[i]=0;
	}
	curr=0;
}


List::~List(){
/*
	Elem* temp=head;
	while(temp!=0) {
		Elem* prev=temp;
		temp=temp->next;
//		delete prev->info; //sada u dealocatePcbs
		delete prev;
	}
	head=0;
*/

	//new:
	lockHard;
	delete[] pcbVect;
	unlockHard;
	pcbVect=0;
}

void List::dealocatePcbs(){
/*	Elem* temp=head;
	while(temp!=0){
		delete temp->info;
		temp=temp->next;
	}
*/
	//new:
	for(int i=0;i<size;i++){
		if(pcbVect[i]!=0){
			lockHard;
			delete pcbVect[i];
			unlockHard;
		}
	}
}

PCB* List::getAndRemove() {
/*
	if(head==0) return 0;
	PCB* retval=head->info;
	Elem* temp=head;
	head=head->next;
	delete temp;
	return retval;
	*/
	for(int i=0;i<size;i++){
		if(pcbVect[i]!=0){
			curr=i;
			PCB*retval=pcbVect[i];
			pcbVect[i]=0;
			return retval;
		}
	}
	return 0;
}

void List::removeById(int id) {
/*
	Elem* prev=0;
	Elem* temp=head;
	while(temp!=0){
		if(temp->info->id==id){
			if(prev==0){
				head=head->next;
			}
			else{
				prev->next=temp->next;
			}
			delete temp;
			return;
		}
		prev=temp;
		temp=temp->next;
	}
*/
	if(id>=size) return;
	pcbVect[id]=0;
}


EvElem::EvElem(KernelEv* i, EvElem* n) {
	info=i;
	next=n;
}

SemElem::SemElem(KernelSem* i, SemElem* n) {
	info=i;
	next=n;
}

EventList::EventList() {
	head=0;
	//new:
	lockHard;
	eventVect=new KernelEv*[START_SIZE];
	unlockHard;
	size=START_SIZE;
	for(int i=0;i<START_SIZE;i++){
		eventVect[i]=0;
	}

}

EventList::~EventList() {
/*
	EvElem* temp=head;
	while(temp!=0) {
		EvElem* prev=temp;
		temp=temp->next;
		delete prev->info;
		delete prev;
	}
	head=0;
*/
	lockHard;
	for(int i=0;i<size;i++){
		if(eventVect[i]!=0) delete eventVect[i];
	}
	delete [] eventVect;
	eventVect=0;
	unlockHard;
}

KernelEv* EventList::operator [](int id) {
/*	EvElem* temp=head;
	while(temp!=0){
		if(temp->info->id==id) return temp->info;
		temp=temp->next;
	}
	return 0;
	*/
	//new:
	if(id<size) return eventVect[id];
		return 0;
}

void EventList::insert(KernelEv* kev) {
/*
	if(head==0)	head=new EvElem(kev,0);
	else{
		EvElem* temp=new EvElem(kev, head);
		head=temp;
	}
	*/

	//new:
	lockHard;
	if(kev->id>=size) {
			KernelEv** newVect=new KernelEv*[2*size];
			for(int i=0;i<size*2;i++){
				if(i<size) newVect[i]=eventVect[i];
				else newVect[i]=0;
			}
			size*=2;
			delete[] eventVect;
			eventVect=newVect;
		}
		eventVect[kev->id]=kev;
		unlockHard;
}

SemList::SemList() {

	head=0;

	//new:
	semVect=new KernelSem*[START_SIZE];
	size=START_SIZE;
	for(int i=0;i<START_SIZE;i++){
		semVect[i]=0;
	}

}

SemList::~SemList() {
/*
	SemElem* temp=head;
	while(temp!=0) {
		SemElem* prev=temp;
		temp=temp->next;
		delete prev->info;
		delete prev;
	}
	head=0;
*/

	//new:
	lockHard;
	for(int i=0;i<size;i++){
		if(semVect[i]!=0) delete semVect[i];
	}
	delete [] semVect;
	semVect=0;
	unlockHard;
}

KernelSem* SemList::operator [](int id) {
/*
	SemElem* temp=head;
	while(temp!=0){
		if(temp->info->id==id) return temp->info;
		temp=temp->next;
	}
	return 0;
*/
	//new:
	if(id<size) return semVect[id];
		return 0;
}

void SemList::insert(KernelSem* kes) {
/*
	if(head==0)	head=new SemElem(kes,0);
	else{
		SemElem* temp=new SemElem(kes, head);
		head=temp;
	}
*/
	//new:
	lockHard;
	if(kes->id>=size) {
			KernelSem** newVect=new KernelSem*[2*size];
			for(int i=0;i<size*2;i++){
				if(i<size) newVect[i]=semVect[i];
				else newVect[i]=0;
			}
			size*=2;
			delete[] semVect;
			semVect=newVect;
		}
		semVect[kes->id]=kes;
		unlockHard;
}


