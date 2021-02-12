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
#include <atomic>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <climits>
#include <limits>
#include <stdint.h>
#include <omp.h>
#include "mergesort.h"


int main_merge[50000];

struct arg_struct {
    int arg1;
    int arg2;
};

using namespace std;
size_t* args;
int num_count = 0;
int num_threads = 0;
vector<int> sortarray;

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
    cout << "Num count = " << num_count << endl;
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


/* @Function name : main
 * @Description : Initialises the variables and calls other functions
 * @Return value : int 0 is no error, int 1 is errors present
 */

int main(int argc, char **argv)
{
	
	//Initialising the variables
	int name_flag = 0; 
	int outfile_flag = 0;
	int i = 0, j = 0, out_flag = 0;
	int read_file;
    int ret = 0;
	int start_index = 0;
	int end_index = 0;
	
    string write_file;
	
	string name = "Sarayu Managoli";
	
        const option long_opts[] = {
            {"name", no_argument, nullptr, 'n'}
	};

	while (true)
	{
		const auto opt = getopt_long(argc, argv, "no:", long_opts, nullptr);

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

                    default:
                            cout << "Invalid!" << endl;
		}
	}
    
    //If there are no arguments passed
	if(argc < 2 || argc > 4)
	{
		if(argc < 2)
			cout << "No arguments passed!" << endl;
		if(argc > 4)
			cout << "Too many arguments passed!" << endl;
		cout << "Usage: ./mysort [--name] [sourcefile.txt] [-o outfile.txt]" << endl;
		return 1;
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
    
	//Display name
	else if(name_flag == 1) 
	{
		cout << name << endl;
		return 0;
	}
    
	//Print unsorted array
	cout << "Unsorted array is:" << endl;
	for(j=0;j<sortarray.size();j++)
	{
		cout << sortarray[j] << endl;
	}
   
    sort_merge(sortarray, 0, sortarray.size() - 1);
    
	//Print sorted array
	cout << "Sorted array is:" << endl;

	for(int j=0;j<sortarray.size();j++)
	{
		cout << sortarray[j] << endl;
	}

	//Check for -o but no mention of output file
	if(out_flag == 1 && outfile_flag == 0)
	{
			cout << "\nUsage: ./mysort [--name] [sourcefile.txt] [-o outfile.txt]\nOutput file not specified!" << endl;
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
	return 0;
}