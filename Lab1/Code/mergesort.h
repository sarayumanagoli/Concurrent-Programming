/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the header implementation for merge sort
 */

#ifndef MERGESORT_H
#define MERGESORT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>
#include <pthread.h>
#include <atomic>
#include <algorithm>
using namespace std;

void mergefunction(vector<int>& sortarray, int left, int mid, int right);
void sort_merge(vector<int>& sortarray, int left, int right);

#endif
