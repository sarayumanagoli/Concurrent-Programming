/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for the implementation of the final project
 * References: https://stackoverflow.com/questions/25833541/reading-numbers-from-file-c
 *	           https://codeyarns.com/2015/01/30/how-to-parse-program-options-in-c-using-getopt_long/
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>
#include <atomic>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <climits>
#include <limits>
#include <stdint.h>
#include "bst.h"

using namespace std;

int hoh_flag = 0;
int rw_flag = 0;

typedef struct thread_arg
{
	int ID;
	int thread_node;
	int thread_insert;
}thread_arg;

thread_arg *thread_args;
pthread_barrier_t bar;
struct timespec startTime, endTime;
int total_threads = 10;
int total_nodes = 10;

/* Function name: mutex_initialization
 * Description: The function is used to initialise all locks and mutexes needed for the multithreading operation.
 */

void mutex_initialization(void)
{
    pthread_mutex_init(&fine_lock, NULL); //Initialize mutex for tree
    pthread_rwlock_init(&rwlock, NULL); //Initialize reader/writer lock for tree
    pthread_barrier_init(&bar, NULL, total_threads); //Initialize barrier
}

/* Function name: free_tree
 * Description: Frees the entire tree that is malloc'd including the child nodes
 */

void free_tree(bstree *root)
{
	if (root != NULL) {
        free_tree(root->right);
        free_tree(root->left);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
}

/* Function name: assign_value
 * Description: The function is used to generate various random key and values required for put, get and delete operation.
 */

void assign_value(int* key, int* value)
{
    *key = rand() % 300; 
    *value = rand() % 300; 
}


std::vector<std::vector<int>> vector_keys;
std::vector<std::vector<int>> vector_values;


/* Function name: low_contention
 * Description: The function is used to demonstrate low contention between the threads generating various key and values required for put, get and delete operation.
 *              Here, each thread randomly searches for values using the get function.
 */

void *low_contention(void *arg)
{
    int key, value;
    thread_arg *t_arg = (thread_arg *)arg;

    pthread_barrier_wait(&bar); 
    if(t_arg->ID == 0) 
    {
        clock_gettime(CLOCK_MONOTONIC,&startTime);
    }
    
    //the values are assigned to their specific keys
    for(int i = 0; i < t_arg->thread_node; i++)
    {
        assign_value(&key,&value);
        vector_keys[t_arg->thread_insert].push_back(key);
        put(NULL, key, value);
    }

    
    pthread_barrier_wait(&bar); 
    int iteration = vector_keys[t_arg->thread_insert].size(); 
    
    //the values are traversed and the bst_get function is used to search for different keys based on the vector_keys array
    for(int i = 0; i < iteration; i++)
    {
        key = vector_keys[t_arg->thread_insert].at(i); 
        bst_get(NULL, key); 
    }
    

    pthread_barrier_wait(&bar); 
    if(t_arg->ID == 0)
    { 
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }
//     free(t_arg);
    return 0;
}

/* Function name: high_contention
 * Description: The function is used to demonstrate high contention between the threads generating various key and values required for put, get and delete operation.
 *              Here, one value is put onto the BST and that value is required to be searched by all the threads. This creates a high contention for that one particular
 *              value thus demonstating the phenomenon.
 */

void *high_contention(void *arg)
{
    int key, value;
    thread_arg *t_arg = (thread_arg *)arg;

    pthread_barrier_wait(&bar); 
    if(t_arg->ID == 0) 
    {
        clock_gettime(CLOCK_MONOTONIC,&startTime);
    }

    //random values and keys are generated and added onto to the BST
    for(int i = 0; i < t_arg->thread_node; i++)
    {
        assign_value(&key,&value); 
        put(NULL, key, value);
    }
    
    //A specific value is added to the BST
    if(t_arg->ID == 0)
        put(NULL, 123, 456); 

    pthread_barrier_wait(&bar); 

    for(int i = 0; i < t_arg->thread_node; i++)
    {
        //All the threads are forced to search for the last specific value input, this increases contention
        bst_get(NULL, 123); 
    }

    pthread_barrier_wait(&bar);
    if(t_arg->ID == 0) 
    {
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }
//     free(t_arg);
    return 0;
}

/* Function name: close
 * Description: This function frees the required memory and destroys locks and barriers.
 */

void close(void)
{
        free(thread_args);
        free_tree(root);
        
        pthread_mutex_destroy(&fine_lock);
        pthread_barrier_destroy(&bar);
        pthread_rwlock_destroy(&rwlock);

}

/* Function name: main_thread_handler
 * Description: The function is used to demonstrate all the basic functionalities on the BST. Here, depending upon the thread ID, the function either gets, puts or
 *              deleted values from the BST.
 */

void *main_thread_handler(void* arg)
{
    int key, value;
    thread_arg *t_arg = (thread_arg *)arg;
    
    if(t_arg->ID == 0)
    {
            clock_gettime(CLOCK_MONOTONIC,&startTime);
    }
    
    pthread_barrier_wait(&bar);
    //The threads are divided into three.
    //The first 'if' condition will consist of the first of the three sets.
    //The first section performs the insertion of various nodes.
    //The second section performs a search of random keys generated.
    //The third section performs the deletion based on the key, value generated using the rand() function.
    //These three sections are divided using the modulo 3 opertion.
    
    if((t_arg->ID % 3) == 0) 
    {
        for(int i = 0; i < t_arg->thread_node; i++)
        {
            assign_value(&key,&value); 
            put(NULL, key, value); //Insert
        }
    }
    else if((t_arg->ID % 3) == 1) 
    {
        for(int i = 0; i < t_arg->thread_node; i++)
        {
            key = rand() % 300; //random key
            value = 0; 
            if((value = bst_get(NULL, key)) != -1) //Search
            {
//                 cout << "Value is "<< value << " for key " << key << endl;
            }
        }
    }
    
    else if((t_arg->ID % 3) == 2) //threads will perform deletion
    {
        for(int i = 0; i < t_arg->thread_node; i++)
        {
            assign_value(&key,&value); 
            root = deleteNode(root,key);
        }
    }

    pthread_barrier_wait(&bar);
    if(t_arg->ID == 0)
    {
            clock_gettime(CLOCK_MONOTONIC,&endTime);
    }
    return arg;
}


/* Function name: main
 * Description: Initialises the variables and threads and calls other functions.
 */

int main(int argc, char **argv)
{
    //Initialising the variables
    int name_flag = 0; 
    int outfile_flag = 0;
    int i = 0, j = 0, out_flag = 0;
    int test_flag = 0;
    unsigned long long time_ns;
    double time_s;
    
    string name = "Sarayu Managoli";

    const option long_opts[] = {
        {"name", no_argument, nullptr, 'n'},
        {"node", required_argument, nullptr, 'o'},
        {"lock", required_argument, nullptr, 'l'},
        {"test", required_argument, nullptr, 's'}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, "nht:o:l:s:", long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
            case 'n':
                name_flag = 1;
                break;
                
            case 'h':
                cout << "With 'make', usage: ./binarysearchtree [--name] [--node=NUM_NODES] [-t NUM_THREADS] [--lock=HOH,RW] [--test=low,high]" << endl;
                cout << "With 'make test', usage: ./unittest" << endl;
                exit(0);
                break;

            case 'o':
                total_nodes = atoi(optarg);
                break;

            case 't':
                total_threads = atoi(optarg);
                break;

            case 'l':
                if(string(optarg) == "HOH") hoh_flag = 1;
                else rw_flag = 1;
                break;

            case 's':
                if(string(optarg) == "low") test_flag = 2;
                else test_flag = 1;
                break;

            default:
                    cout << "Invalid!" << endl;
        }
    }

    //If there are no arguments passed
    if(argc < 2 || argc > 6)
    {
        if(argc < 2)
            cout << "No arguments passed!" << endl;
        if(argc > 4)
            cout << "Too many arguments passed!" << endl;
        cout << "Usage: ./binarysearchtree [--name] [--node=NUM_NODES] [-t NUM_THREADS] [--lock=HOH,RW] [--test=low,high]" << endl;
        return 1;
    }
    
    //Display name
    else if(name_flag == 1) 
    {
        cout << name << endl;
        return 0;
    }
    
    else 
    {
        cout << "Hand-over-hand Lock flag = " << hoh_flag << endl;
        cout << "RW Lock flag = " << rw_flag << endl;
        cout << "Number of threads = " << total_threads << endl;
        cout << "Number of nodes = " << total_nodes << endl;

        if(total_threads>total_nodes)
        {
            cout << "Invalid input; the number of threads must not exceed the number of nodes" << endl;
            return 1;
        }
        pthread_t thread_id[total_threads]; 
        thread_args = (thread_arg *)malloc(total_threads * sizeof(thread_arg)); 
        
        //Function called to generate random numbers
        srand(time(0));
        int insert_index = 0, thread_insert = 0, ret = 0; 
        
        //If the contentions are to be checked, thread_insert is equal to the total number of threads
        if(test_flag != 0)
            thread_insert = total_threads;
        else
            thread_insert = (total_threads/3) + 1;
        
        mutex_initialization();
        
        vector_keys.resize(thread_insert); 
        vector_values.resize(thread_insert);
        
        float node_thread = total_nodes/total_threads;
        float node_insert =  total_nodes/thread_insert;
        
        for(int i = 0; i < total_threads; i++) 
        {
            thread_args[i].ID = i;
            
            //Here, high or low contention node values are being calculated. The total nodes are divided between the total threads.
            //This is to accomodate for the phenomenon of contention.
            
            if(test_flag != 0) 
            {
                //This part of the code is executed if one of the contention methods are chosen
                thread_args[i].thread_insert = i; 
                if(thread_args[i].ID == (total_threads - 1))
                    thread_args[i].thread_node = total_nodes - node_thread * (total_threads - 1);
                else
                    thread_args[i].thread_node = node_thread;
            }
            else 
            {
                //This is the normal mode of operation without any contention
                if((thread_args[i].ID % 3) != 0)
                {
                    //One out of three threads undergo this operation
                    thread_args[i].thread_node = node_insert;
                    thread_args[i].thread_insert = -1;
                }
                else
                {
                    //Two out of three threads undergo this operation
                    if((insert_index + 1) != thread_insert)
                    {
                        thread_args[i].thread_node = node_insert;
                    }
                    else
                    {
                        //This calculates the number of nodes that each thread should insert.
                        thread_args[i].thread_node = total_nodes - node_insert * insert_index;
                    }
                    thread_args[i].thread_insert = insert_index; 
                    //The insertion thread index is incremented
                    insert_index++;
                }
            }

            //If test_flag = 0, the user has opted for normal mode of operation
            if(test_flag == 0) 
            {
                ret = pthread_create(&thread_id[i], NULL, main_thread_handler, (void*) &thread_args[i]);
                if(ret)
                {
                    printf("ERROR; main pthread_create: %d\n", ret);
                    exit(-1);
                }
            }
            //If test_flag = 1, the user has opted for high contention mode of operation
            else if(test_flag == 1) 
            {
                ret = pthread_create(&thread_id[i], NULL, high_contention, (void*) &thread_args[i]);
                if(ret)
                {
                    printf("ERROR; high contention pthread_create: %d\n", ret);
                    exit(-1);
                }
            }
            //If test_flag = 2, the user has opted for low contention mode of operation
            else
            {
                ret = pthread_create(&thread_id[i], NULL, low_contention, (void*) &thread_args[i]);
                if(ret)
                {
                    printf("ERROR; low contention pthread_create: %d\n", ret);
                    exit(-1);
                }
            }
        }

        for(int i = 0; i < total_threads; i++) 
        {
            ret = pthread_join(thread_id[i], NULL);
            if(ret)
            {
                printf("ERROR; pthread_join: %d\n", ret);
                exit(-1);
            }
        }
        
        cout << "*******Range Query DEMO*******" << endl;
        cout << "The keys and values present in the range are:" << endl;
        put(NULL, 0, 456); 
        put(NULL, 1, 45); 
        put(NULL, 2, 24);
        put(NULL, 7, 24);
        bst_range_query(root, 0, 10);
        
//         print_tree(root);
        
        //Delete function executed in order to better explain its implementation
        cout << "*******Put, Delete and Get DEMO*******" << endl;
        int value = 0;
        put(NULL, 10, 57);
        if((value = bst_get(NULL, 10)) == -1) //Search
        {
            cout << "Before delete: Unable to find " << 10 << " in the BST" << endl; 
        }
        else
        {
            cout << "Before delete: Value is "<< value << " for key " << 10 << endl;
        }
        root = deleteNode(root,10);
        if((value = bst_get(NULL, 10)) == -1) //Search
        {
            cout << "After delete: Unable to find " << 10 << " in the BST" << endl; 
        }
        else
        {
            cout << "After delete: Value is "<< value << " for key " << 10 << endl;
        }
        
        close();

        //Time calculation
        time_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
        time_s = ((double)time_ns)/1000000000.0;
        cout << "*******Time Analysis*******" << endl;
        printf("Time taken in ns: %llu\n",time_ns);
        printf("Time taken in s: %lf\n",time_s);
        
 
    }
    return(0);
}