/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for the counter implementation of Lab 2
 * References: https://stackoverflow.com/questions/25833541/reading-numbers-from-file-c
 *             https://www.geeksforgeeks.org/merge-sort/
 *	           https://codeyarns.com/2015/01/30/how-to-parse-program-options-in-c-using-getopt_long/
 */


#include <mutex> 
#include <atomic>
#include <iostream> 
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <getopt.h>
#include <set>
#include <thread>
#include <unistd.h>
#include <map>
#include <stdbool.h>
#include <climits>
#include <limits>
#include <stdint.h>
#include "algorithm.h"

using namespace std;

pthread_barrier_t bar;
pthread_mutex_t mutexLock;
int lock_flag = 0;
pthread_mutex_t thread_lock;


int num_threads;
int num_iterations;
int count_main = 0;

struct timespec startTime, endTime;

MCSLock mcs_lock;
atomic<Node*> tail_mcs{NULL};


/* @Function name : write_to_file
 * @Description : Writes the integer values from the arrays and stores it in a file as mentioned 
 *                command line argument
 * @Return value : int
 */

int write_to_file(string write_file_name, int cnt)
{
	ofstream outfile;
	outfile.open(write_file_name);
	int i = 0;

	if(!outfile)
	{
		return 1;
	}

	outfile << cnt << endl;
	outfile.close();	
	return 0;
}

void* thread_count(void* args)
{
	int tid = *((int *)args);
	int i = 0;
	Node *mynode = new Node;
	cout << "####In thread " << tid << "####" << endl;
	
	if(tid == 0)
		clock_gettime(CLOCK_MONOTONIC,&startTime);
      
	for (i = 0; i < (num_iterations * num_threads); i++)
	{
		if ((i % num_threads) == tid)
		{
			//cout << "In counter thread" << endl;
			switch(lock_flag)
			{
				case 0: tasLock(); break;
				case 1: ttasLock(); break;
				case 2: ticketLock(); break;
				case 3: pthread_mutex_lock(&thread_lock); break;
				case 4:	//cout << "MCS lock" << endl;
					mcs_lock.acquire(mynode);
					break;
			}
			count_main++;
			cout <<"Thread = " << tid << " Count = " << count_main << endl;
			switch(lock_flag)
			{
				case 0: tasUnlock(); break;
				case 1: ttasUnlock(); break;
				case 2: ticketUnlock(); break;
				case 3: pthread_mutex_unlock(&thread_lock); break;
				case 4: //cout << "MCS unlock" << endl;
					mcs_lock.release(mynode); 
					break;
			}
		}
         if(lock_flag == 5)
        {
            //cout << "sense bar wait" << endl;
            sense_reverse();
        }


        if(lock_flag == 6)
        {
            cout << "pthread_bar_wait ended" << endl;
            pthread_barrier_wait(&bar); 
        }
	}
    
    /*if(lock_flag == 5)
    {
        cout << "sense bar wait" << endl;
        sense_reverse();
    }*/


    if(lock_flag == 6)
    {
        cout << "pthread_bar_wait ended" << endl;
        pthread_barrier_wait(&bar); 
    }
    
   
	if(tid == 0)
		clock_gettime(CLOCK_MONOTONIC,&endTime);

	return 0;
}

int main(int argc, char* argv[]) 
{
  	(void)argc;

	int name_flag = 0, outfile_flag = 0;
	int pthreadbar_flag = 0, sense_flag = 0, out_flag = 0; 
        string write_file;
    int flag_duplicate = 0;
	int i = 0;
	string name = "Sarayu Managoli";
	unsigned long long time_ns;
	double time_s;

    	const option long_opts[] = {
            {"name", no_argument, nullptr, 'n'},
            {"bar", required_argument, nullptr, 'b'},
	    {"lock", required_argument, nullptr, 'l'}
	};

	while (true)
	{
		const auto opt = getopt_long(argc, argv, "nt:i:blo:", long_opts, nullptr);

		if (-1 == opt)
		    break;

		switch (opt)
		{
			case 'n':
				name_flag = 1;
				break;

			case 'o':
  				if(optarg)
				{
					write_file = string(optarg);
					outfile_flag = 1;
				}
				else
				{
					cout << "No output file mentioned" << endl;
				}
					out_flag = 1;
				break;

			case 'l':
				if(string(optarg) == "tas")
                {
                    lock_flag = 0;
                    cout << "***TAS LOCK***" << endl;
                }
				if(string(optarg) == "ttas") 
                {
                    lock_flag = 1;
                    cout << "***TTAS LOCK***" << endl;
                }
				if(string(optarg) == "ticket") 
                {
                    lock_flag = 2;
                    cout << "***TICKET LOCK***" << endl;
                }	
				if(string(optarg) == "pthread")
                {
                    lock_flag = 3;
                    cout << "***PTHREAD LOCK***" << endl;
                }
				if(string(optarg) == "mcs") 
                {
                    lock_flag = 4;
                    cout << "***MCS LOCK***" << endl;
                }
                flag_duplicate = 1;
				break;

			case 't':
				num_threads = atoi(optarg);
                                cout << "Thread count is " << num_threads << endl;
				break;

			case 'i':
				num_iterations = atoi(optarg);
                                cout << "Number of iterations in each thread is " << num_iterations << endl;
				break;
			case 'b':
				if(string(optarg) == "sense") 
                {
                    lock_flag = 5;
                    cout << "***SENSE-REVERSAL BARRIER***" << endl;
                }
				if(string(optarg) == "pthread") 
                {
                    lock_flag = 6;
                    cout << "***PTHREAD BARRIER***" << endl;
                }
				break;
                        default:
                                cout << "Invalid!" << endl;
		}
	}

	pthread_t thread[num_threads];
	int thread_id[num_threads];	

	if(lock_flag == 6)
	pthread_barrier_init(&bar, NULL, num_threads);  

	if (pthread_mutex_init(&thread_lock, NULL) != 0) 
	{
    		cout << "Mutex Init Failed!" << endl;
    	}

	if(num_threads > 150)
	{
		cout << "Thread count is too high. Please provide a value lesser than or equal to 150." << endl;
		return 1;
	}
	

	//Display name
	else if(name_flag == 1) 
	{
		cout << name << endl;
		return 0;
	}

    if(flag_duplicate == 1 && lock_flag >=5)\
    {
        cout << "Invalid input! Please provide either lock or barrier!" << endl;
        return 1;
    }
    
	int ret = 0;
	for(i=0; i<num_threads; i++)
	{	
		thread_id[i] = i;
		ret = pthread_create(&thread[i], NULL, thread_count, &thread_id[i]);
		if (ret) 
		{
			cout << "Error on creating the thread" << endl;
			exit(1);
		} 
		else {
			cout << "Creating thread " << (i+1) << endl;
		}

	}

	for(i=0; i<num_threads; i++)
	{
		cout << "Joining thread" << i << endl;;
		pthread_join(thread[i], NULL);
	}
	cout << "Counter value = " << count_main << endl;

	time_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	time_s = ((double)time_ns)/1000000000.0;
	printf("Time taken in ns: %llu\n",time_ns);
	printf("Time taken in s: %lf\n",time_s);
	
	if(lock_flag == 6)
	pthread_barrier_destroy(&bar);	
	
	pthread_mutex_destroy(&thread_lock);

	//Check for -o but no mention of output file
	if(out_flag == 1 && outfile_flag == 0)
	{
			cout << "\nUsage: ./counter [--name] [-t NUMTHREADS] [-i=NUMITERATIONS][--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,mcs,pthread>] [-o out.txt]\n\nOutput file not specified!" << endl;
			return 1;
	}
	

	//Check for -o with mention of output file
	else if(out_flag == 1 && outfile_flag == 1)
	{
			if(write_to_file(write_file,count_main))
			{	
				cout << "\nError opening output file. Provide a valid file." << endl;
				return 1;
			}
			else
				cout << "\nOutput successfully written to the specified file!" << endl;
		
	}
	//No mention of output file
	else
	{
		cout << "\nOutput file not specified. Output successfully written to stdout!" << endl;
	}

    return 0;
}
