/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the header code for the BST implementation for Final project
 * References: https://codeyarns.com/2015/01/30/how-to-parse-program-options-in-c-using-getopt_long/
 *             https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
 *             https://www.tutorialspoint.com/binary-search-tree-delete-operation-in-cplusplus
 */

#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
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

typedef struct bstree
{
    int key;
    int value;
    struct bstree *left;
    struct bstree *right;
    pthread_mutex_t lock;
    pthread_rwlock_t rwlock;
}bst_node;

extern int hoh_flag;
extern int rw_flag;
extern bst_node *root;
extern pthread_mutex_t fine_lock;
extern pthread_rwlock_t rwlock;

bstree *put(bstree *func_node, int key, int value);
bstree *new_node(int key, int value);
int bst_get(bstree *func_node, int key);
bstree *get(bstree *func_node, int key);
void bst_range_query(bstree* node, int start_key, int end_key);
bstree* deleteNode(bstree* func_node, int key);
void print_tree(bstree *root);

#endif