/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for the lock and barrier implementation of Lab 2
 * References: https://mfukar.github.io/2017/09/26/mcs.html
 *             https://geidav.wordpress.com/2016/03/23/test-and-set-spinlocks/
 *             https://stackoverflow.com/questions/33048079/test-and-test-and-set-in-c
 *             Lecture slides
 */

#include "algorithm.h"

atomic_bool lockflag (false);
/*bool tas()
{
    if(lockflag == false)
    {
        lockflag = true; 
        return true;
    }
    else
    {
        return false;
    }
}

void tasLock()
{
    int local_flag = false;
    if(lockflag == false)
    {
        local_flag = true; 
    }
    else
    {
        local_flag = false;
    }
    cout << "TASLock" << endl;
    while(local_flag == false)
    { //cout << "TASLocked!!" << endl;
    } 
}

void tasUnlock()
{
    cout << "TASUnlock" << endl;    
    lockflag.store(false, memory_order_seq_cst);
}*/

void tasLock()
{
    cout << "TASLock" << endl;
	bool val_e, val_c;
    do
    {
        val_c = true;
        val_e = false;
    }while(!lockflag.compare_exchange_strong(val_e,val_c));
}
void tasUnlock()
{
	cout << "TASUnlock" << endl;
	lockflag.store(false);
}

void ttasLock()
{
    cout << "TTASLock" << endl;
    do {
    	while (lockflag.load()) {}
    } while (lockflag.exchange(true));
    
    return;
}

void ttasUnlock()
{
	cout << "TTASUnlock" << endl;
	lockflag.store(false);
}

/*void ttasLock()
{
    cout << "TTASLock" << endl;
    while(lockflag.load())
    {
        while (lockflag.exchange(true)) {}
    }
}

void ttasUnlock()
{
	cout << "TTASUnlock" << endl;
	lockflag.store(false);
}*/


atomic_int next_value       (0);
atomic_int now_serving    (0);

void ticketLock()
{
	cout << "TicketLock" << endl;
    int present_value = atomic_fetch_add_explicit(&next_value, 1, memory_order_seq_cst);
	while(now_serving.load(memory_order_seq_cst) != present_value)
	{}
}
void ticketUnlock() 
{
	cout << "TicketUnlock" << endl;
	atomic_fetch_add_explicit(&now_serving, 1, memory_order_seq_cst);
}


atomic_int sense (0);
atomic_int count (0);

void sense_reverse()
{
	//cout << "In SRB wait" << endl;
	thread_local bool my_sense = 0;
	if(my_sense == 0) 
        my_sense = 1;
	else 
        my_sense = 0;
	int count_duplicate = 0; //number of threds that have arrived at the barrier
    count_duplicate = atomic_fetch_add(&count, 1);
	if (count_duplicate == num_threads - 1)       //last to arrive            
	{
		count.store(0, memory_order_relaxed);
		sense.store(my_sense);
	}
	else                                    //not last
	{
		while(sense.load() != my_sense);
	}
}
