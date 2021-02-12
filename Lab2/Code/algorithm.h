/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the header implementation of lock and barrier
 */ 

#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <iostream>       
#include <atomic>         
#include <thread>         
#include <vector>         
#include <sstream>   
#include <assert.h>

using namespace std;

extern int num_threads;

class Node
{
public:
	atomic<Node*> next;
	atomic<bool> wait;
};

extern atomic<Node*> tail_mcs;


class MCSLock {
public:
	void release(Node *myNode) 
    {

		Node* node_temp = myNode;
		if (tail_mcs.compare_exchange_strong(node_temp, NULL)) 
        {} 
        else 
        {
			while (myNode->next.load() == NULL) {}
			myNode->next.load()->wait.store(false);
		}
	}
    
    void acquire(Node *myNode) 
    {
        Node *oldTail = tail_mcs.load();
        myNode->next.store(NULL, memory_order_relaxed);
        while (!tail_mcs.compare_exchange_strong(oldTail, myNode)) 
        {
            oldTail = tail_mcs.load();
        }

        if (oldTail != NULL) 
        {
            assert(myNode != oldTail);
            myNode->wait.store(true, memory_order_relaxed);
            oldTail->next.store(myNode);
            while (myNode->wait.load()) 
            {}
        }
	}
};


//bool tas(void);
void tasLock(void);
void tasUnlock(void);
void ttasLock(void);
void ttasUnlock(void);
void ticketLock (void);
void ticketUnlock(void);
void sense_reverse(void);

#endif
