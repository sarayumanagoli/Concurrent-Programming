/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for the unit test of the final project
 * References: https://stackoverflow.com/questions/25833541/reading-numbers-from-file-c
 *	           https://codeyarns.com/2015/01/30/how-to-parse-program-options-in-c-using-getopt_long/
 */

#include "bst.h"
#include <stdbool.h>
#include <assert.h>
#include <iostream>

using namespace std;

int hoh_flag = 0;
int rw_flag = 1;

void free_tree(bstree *root)
{
	if (root != NULL) {
        free_tree(root->right);
        free_tree(root->left);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
 }

void test_range()
{
    printf("Between 0 and 10, the tree has the values:\n");
    bst_range_query(root, 0, 10);
}

bool test_delete()
{
    int i = 0;
    for (i = 0; i < 50; i++) 
    {
        put(NULL, i, i+1);
    }
    
    for(i = 0; i<50;i++)
    {
        root = deleteNode(root,i%5);
    }
    
    for(i = 0; i<50;i++)
    {
        if(bst_get(NULL, i%5) != -1)
        return false;
    }
    return true;   
}


void BST_Array(bstree *root, int* arr)
{
    static int i = 0;
    if(root == NULL) 
        return;

    BST_Array(root->left, arr);
    arr[i] = root->key;
    i++;
    BST_Array(root->right, arr);

}

bool test_put()
{
    int i = 0;
    for (i = 0; i < 100; i++) 
    {
        put(NULL, i, (i+1));
    }

    int *arr = (int*)malloc(100*sizeof(int));
    BST_Array(root, arr);

    for (i = 0; i < 100; i++) 
    {
        assert(i == arr[i]);
    }
    free(arr);
    return true;
}

bool test_get()
{
    int i = 0;
    bstree *node;

    for (i = 0; i < 50; i++) {
        node = get(NULL, i);
        if (node == NULL)
            return false;
        assert(node->value == (i+1));
    }
    return true;
}

int main()
{
    if (test_put()) {
        cout << "BST put function test passed!" << endl;
    }
    if (test_get()) {
        cout << "BST get function test passed!" << endl;
    }
    if (test_delete()) {
        cout << "BST delete function test passed!" << endl;
    }
    
    cout << "\nBST range function test initiated!" << endl;
    test_range();
    
    free_tree(root);

    return 0;
}