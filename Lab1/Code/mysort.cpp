/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for the implementation of Lab 1
 * References: https://stackoverflow.com/questions/25833541/reading-numbers-from-file-c
 *             https://www.geeksforgeeks.org/merge-sort/
 *	       https://codeyarns.com/2015/01/30/how-to-parse-program-options-in-c-using-getopt_long/
 * 	       https://www.geeksforgeeks.org/bucket-sort-2/
 *             https://stackoverflow.com/questions/52767944/merge-sort-with-pthreads-in-c
 *	       https://stackoverflow.com/questions/48965540/how-do-i-insert-arrays-as-values-into-map-in-c
 * 	       https://dyclassroom.com/sorting-algorithm/bucket-sort
 */


//Defines

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>
#include <pthread.h>
#include <atomic>
#include <algorithm>
#include <math.h>
#include <set>
#include <unistd.h>
#include <map>
#include <stdbool.h>
#include <climits>
#include <limits>
#include <stdint.h>
#include "mergesort.h"


int main_merge[50000];
struct timespec startTime, endTime;

struct arg_struct {
    int arg1;
    int arg2;
};

using namespace std;
size_t* args;
int num_count = 0;
atomic_int part (0);  
pthread_barrier_t bar; 
int thread_cnt = 0;
vector<int> sortarray;
int array_num = 0;
pthread_mutex_t bucket_lock;
//10 buckets
map<uint64_t, uint64_t> mymap[10];

/* @Function name : read_from_file
 * @Description : Reads the integer values from the file specified in the command line argument
 *                and stores it in an array
 * @Return value : int
 */

int read_from_file(char *read_file_name,vector<int> &readarray)
{
	int num;
	ifstream infile;
	infile.open(read_file_name);
	
	if(!infile)
	{
		return 1;
	}

	if (infile.is_open())
	{ 
		while(infile >> num)
		{
			readarray.push_back(num);
			num_count++;
		}
	}
	cout << "Num count = " << num_count <<endl;
	infile.close();
	return 0;
}


/* @Function name : write_to_file
 * @Description : Writes the integer values from the arrays and stores it in a file as mentioned 
 *                command line argument
 * @Return value : int
 */

int write_to_file(string write_file_name,vector<int> &writearray)
{
	ofstream outfile;
	outfile.open(write_file_name);
	int i = 0;

	if(!outfile)
	{
		return 1;
	}

	for(i=0;i<writearray.size();i++)
	{	
		outfile << writearray[i] << endl;
	}

	outfile.close();	
	return 0;
}


/* @Function name : getMax
 * @Description : This function returns the maximum value in an array
 * @Return value : int
 */


int getMax(vector<int>& arr, int size)
{
	int max = 0;
	for(int i =0;i < size; i++)
	{
		if(max<arr[i])
			max = arr[i];
	}
	return max;
}

void* bucket_store(void* arguments)
{
	struct arg_struct *args = (struct arg_struct *)arguments;
	int bucket = 10,divider;
	int start_index = args -> arg1;
	uint64_t a = 0;
	int end_index = args -> arg2;
	if(part == 0)
	clock_gettime(CLOCK_MONOTONIC,&startTime);
	uint64_t i;
	int j;
	part++;	
	/*cout << "Part = " << part << endl;
	cout << "Start index = " << start_index << endl;
	cout << "End index = " << end_index << endl;*/
	//find max	
	int max = getMax(sortarray, sortarray.size());
	divider = (float(max + 1) / bucket);

	//insert element into bucket
	for(i = start_index; i <= end_index; i++) 
	{
		j = floor(sortarray[i] / divider);
		a = sortarray[i];
		//cout << " a = " << a << endl;
		pthread_mutex_lock(&bucket_lock);
		if(j==10) j = 9;
		mymap[j].insert(pair<uint64_t, uint64_t>(a,a));
		pthread_mutex_unlock(&bucket_lock);
	}	
}


void* merge_handler(void* args)
{
	if(part == 0)
	clock_gettime(CLOCK_MONOTONIC,&startTime);
	//size_t tid = *((size_t*)args);
	int threadPart = part++;
	int low, high;
	low = (threadPart * num_count) / thread_cnt;
        //cout << "Low = " << low << endl; 
	high = (((threadPart + 1) * num_count) / thread_cnt) - 1;
        //cout << "High = " << high << endl;
	pthread_barrier_wait(&bar);
	int mid = (int)(low + (high - low) / 2); 
	main_merge[threadPart] = high;
	if (low < high) 
	{ 
		cout << "Thread Part = " << threadPart << endl;
		sort_merge(sortarray,low, mid);
		sort_merge(sortarray,mid + 1, high);
		mergefunction(sortarray,low, mid, high);
	}
}

/* @Function name : main
 * @Description : Initialises the variables and calls other functions
 * @Return value : int 0 is no error, int 1 is errors present
 */

int main(int argc, char **argv)
{
	
	//Initialising the variables
	int name_flag = 0; 
	int outfile_flag = 0;
        int merge_flag = 0; 
	int bucket_flag = 0;
	int i = 0, j = 0, out_flag = 0;
	int read_file;
        int ret = 0;
	int start_index = 0;
	int end_index = 0;
	unsigned long long time_ns;
	double time_s;
	
        string write_file;
	
	string name = "Sarayu Managoli";
	
        const option long_opts[] = {
            {"name", no_argument, nullptr, 'n'},
            {"alg", required_argument, nullptr, 'a'}
	};

	while (true)
	{
		const auto opt = getopt_long(argc, argv, "no:t:a", long_opts, nullptr);

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

			case 'a':
				if(string(optarg) == "fjmerge") merge_flag = 1;
				if(string(optarg) == "lkbucket") bucket_flag = 1;
				break;

			case 't':
				thread_cnt = atoi(optarg);
                                cout << "Thread count is " << thread_cnt << endl;
				break;

                        default:
                                cout << "Invalid!" << endl;
		}
	}

	if (optind < argc)
	{
		//This step is to parse the remaining arguments that are non-option
		while (optind < argc)
		{
			//Check for error while opening file
			if(read_file = read_from_file(argv[optind], sortarray))
			{	
				cout << "\nError opening input file. Provide a valid file." << endl;
				return 1;
			}
			optind++;
		}
		putchar('\n');
	}

	//If there are no arguments passed
	if(argc < 2 || argc > 7)
	{
		if(argc < 2)
			cout << "No arguments passed!" << endl;
		if(argc > 7)
			cout << "Too many arguments passed!" << endl;
		cout << "Usage: ./mysort [--name] [sourcefile.txt] [-o outfile.txt] [--alg=<merge,quick>]" << endl;
		return 1;
	}

	if(thread_cnt > 150)
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

	struct arg_struct args[thread_cnt];

	pthread_barrier_init(&bar, NULL, thread_cnt);

	//Print unsorted array
	cout << "Unsorted array is:" << endl;
	for(j=0;j<sortarray.size();j++)
	{
		cout << sortarray[j] << endl;
	}
	
	pthread_t threads[thread_cnt]; 
	if(merge_flag == 1)
	{
		cout << "\nImplementing Merge Sort" << endl;
		//Check for sorting algorithm choice input
		for(i=0; i<thread_cnt; i++)
		{
			printf("Creating thread %d\n",i);
			ret = pthread_create(&threads[i], NULL, &merge_handler, &args[i]);
			if(ret)
				{
					printf("ERROR; pthread_create: %d\n", ret);
					exit(-1);
				}
		}
		// join threads
		for(size_t i=0; i<thread_cnt; i++)
		{
			ret = pthread_join(threads[i],NULL);
			if(ret)
			{
				printf("ERROR; pthread_join: %d\n", ret);
				exit(-1);
			}
			printf("Joining thread %zu\n",i+1);
			clock_gettime(CLOCK_MONOTONIC,&endTime);
		}

		for (i = 0; i < thread_cnt - 1; i++) 
		{
			//cout << "main merge (i+1) " << main_merge[i+1] << endl;
			mergefunction(sortarray,0, main_merge[i], main_merge[i+1]);
		}
	}
	pthread_barrier_destroy(&bar);
	if(bucket_flag == 1)
	{	
		cout << "\nImplementing Bucket Sort" << endl;
		//Check for sorting algorithm choice input
		
		for(i=0; i<thread_cnt; i++)
		{
			start_index = i*floor(num_count/thread_cnt);
			if((i+1) == thread_cnt)
				end_index = num_count - 1;
			else
				end_index = (i+1)*floor(num_count/thread_cnt) - 1;

			args[i].arg1 = start_index;
    			args[i].arg2 = end_index;

			printf("Creating thread %d\n",i);
			ret = pthread_create(&threads[i], NULL, &bucket_store, &args[i]);
			if(ret)
				{
					printf("ERROR; pthread_create: %d\n", ret);
					exit(-1);
				}
		}
		clock_gettime(CLOCK_MONOTONIC,&endTime);
		// join threads
		for(size_t i=0; i<thread_cnt; i++)
		{
			ret = pthread_join(threads[i],NULL);
			if(ret)
			{
				printf("ERROR; pthread_join: %d\n", ret);
				exit(-1);
			}
			printf("Joined thread %zu\n",i+1);
		}
		
		sortarray.clear();
		for(i= 0; i< 10;i++) 
		{
			for(auto& j:mymap[i]) 
			{
				sortarray.push_back(j.first);
			}
		}
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}

	if(argc < 4 || (bucket_flag == 0 && merge_flag == 0))
	{
		//If no sorting algorithm is mentioned, consider Quick sort
		cout << "\n*****No sorting algorithm specified!*****" << endl;
		cout << "Please mention a sorting algorithm" << endl;
		return 1;
	}
		
	array_num = floor(num_count/thread_cnt);

	//Print sorted array
	cout << "Sorted array is:" << endl;

	for(int j=0;j<sortarray.size();j++)
	{
		cout << sortarray[j] << endl;
	}

	//Check for -o but no mention of output file
	if(out_flag == 1 && outfile_flag == 0)
	{
			cout << "\nUsage: ./mysort [--name] [sourcefile.txt] [-o outfile.txt] [--alg=<merge,quick>]\n\nOutput file not specified!" << endl;
			return 1;
	}
	//Check for -o with mention of output file
	else if(out_flag == 1 && outfile_flag == 1)
	{
			if(write_to_file(write_file,sortarray))
			{	
				cout << "\nError opening output file. Provide a valid file." << endl;
				return 1;
			}
			else
				cout << "\nSorted output successfully written to the specified file!" << endl;
		
	}
	//No mention of output file
	else
	{
		cout << "\nOutput file not specified. Sorted output successfully written to stdout!" << endl;
	}

	if(num_count < thread_cnt) 
		cout << "Please refrain from providing a greater thread count compared to the number of elements. It creates idle threads." << endl;

	//Time calculation
	time_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	time_s = ((double)time_ns)/1000000000.0;
	printf("Time taken in ns: %llu\n",time_ns);
	printf("Time taken in s: %lf\n",time_s);	
	
	return 0;
}
