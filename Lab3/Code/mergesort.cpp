/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for merge sort
 * Reference: http://madhugnadig.com/articles/parallel-processing/2017/02/25/parallel-computing-in-c-using-openMP.html
 */

#include "mergesort.h"
using namespace std;

/* @Function name : mergefunction
 * @Description : Merges the left and the right subarrays onto the main array
 * @Return value : void
 */

void mergefunction(vector<int>& mergearray, int left, int mid, int right) 
{ 
	int i, j, k;

	//Two array lengths are calculated, array before and after the middle element
	int lengthleft = mid - left + 1; 
	int lengthright = right - mid; 
	
	//Two arrays of the above sizes are created 
	int LeftArray[lengthleft];
	int RightArray[lengthright]; 
    
   
        //Arrays are populated
        for (i = 0; i < lengthleft; i++)
        { 
            LeftArray[i] = mergearray[left + i];
        }
        for (j = 0; j < lengthright; j++)
        { 
            RightArray[j] = mergearray[mid + 1 + j]; 
        }

        //k is the first index of sub array
        k = left; 
        //i is the first index of LeftArray
        i = 0;
        //j is the first index of RightArray  
        j = 0; 

        //Populate the main array using the left and right arrays
        while (i < lengthleft && j < lengthright) 
        { 
            //if left array element is greater than the right array element, merge that first
            if (LeftArray[i] > RightArray[j]) 
            { 
                mergearray[k] = RightArray[j]; 
                j++; 	 
            } 
            else 
            { 
                mergearray[k] = LeftArray[i]; 
                i++;
            } 
            k++; 
        } 

        //Adding remaining elements onto the main array
        while (j < lengthright) 
        { 
            mergearray[k] = RightArray[j];
            k++; 
            j++;  
        } 
        while (i < lengthleft) 
        { 
            mergearray[k] = LeftArray[i]; 
            i++; 
            k++; 
        }
   
} 
  

/* @Function name : sort_merge
 * @Description : Implements merge sort by arranging the elements in an ascending order
 * @Return value : void
 */

void sort_merge(vector<int>& mergearray, int left, int right) 
{ 
    int mid = 0;
    if (left < right) 
    { 
        mid = (left + right) / 2; 
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                //Sort first and the second half of the entire array 
                sort_merge(mergearray, left, mid); 
            }
            #pragma omp section
            {
                sort_merge(mergearray, mid + 1, right); 
            }
        }
        mergefunction(mergearray, left, mid, right); 
    }
} 



