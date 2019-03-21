/*
 * list.h
 *
 *  Created on: Jun 3, 2018
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_

class PCB;
class KernelEv;
class KernelSem;

class Elem{
public:
	PCB* info;
	Elem* next;
	Elem(PCB* i, Elem* n=0);
};


class List {
	Elem* head;
	int curr;
	PCB** pcbVect;
	int size;

public:
	List();
	PCB* operator[](int);
	void insert(PCB*);
	PCB* getAndRemove();
	void removeById(int id);
	void dealocatePcbs();
	~List();
};


class EvElem{
public:
	KernelEv* info;
	EvElem* next;
	EvElem(KernelEv* i, EvElem* n=0);
};


class EventList{
	EvElem* head;
	int size;
	KernelEv**eventVect;
public:
	EventList();
	~EventList();
	KernelEv* operator[](int);
	void insert(KernelEv* kev);
};

class SemElem{
public:
	KernelSem* info;
	SemElem* next;
	SemElem(KernelSem* i, SemElem* n=0);
};



class SemList{
	SemElem* head;
	int size;
	KernelSem** semVect;
public:
	SemList();
	~SemList();
	KernelSem* operator[](int);
	void insert(KernelSem* kev);
};


#endif /* H_LIST_H_ */
