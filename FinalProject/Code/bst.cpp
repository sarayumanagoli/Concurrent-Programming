/* 
 * Author: Sarayu Managoli
 * Course: Concurrent Programming
 * Overview: This file contains the source code for the BST implementation for Final project
 * References: https://codeyarns.com/2015/01/30/how-to-parse-program-options-in-c-using-getopt_long/
 *             https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
 *             https://www.tutorialspoint.com/binary-search-tree-delete-operation-in-cplusplus
 */



#include "bst.h"
using namespace std;

/* Function name: print_tree
 * Description: The function is used to print all the nodes with their respective keys. It is done inorder.
 */

void print_tree(bstree *root)
{
    static int i = 0;
    if (root == NULL)
        return;

    print_tree(root->left);
    printf("Key: %d Value: %d Count: %d\n", root->key, root->value, i++);
    print_tree(root->right);

}

/* Function name: new_node
 * Description: The function is used to create a new node if there is a requirement.
 *              It provides a null to the left and the right child nodes upon initialisation.
 */

bstree *new_node(int key, int value)
{
    bstree *local = (bstree *)malloc(sizeof(bstree)); 
    if(local == NULL) 
    {
        printf("Failed to allocate memory for new node:%s.\n", strerror(errno));
        return local;
    }

    local->key = key; //Assign key to node
    local->value = value; //Assign value to node
    local->left = NULL; //Empty leaf nodes
    local->right = NULL; //Empty leaf nodes
    pthread_mutex_init(&local->lock, NULL);
    pthread_rwlock_init(&local->rwlock, NULL); 
    return local; //return node pointer
}

// bstree* FindMax(bstree* root)
// {
//     if(root==NULL)
//     return NULL;

//     while(root->right != NULL)
//     {
//         root = root->right;
//     }
//     return root;
// }

// bstree* deleteNode(bstree* func_node,int key)
// {
//     if(func_node==NULL) return root;
//     else if(key<func_node->value) 
//         func_node->left = deleteNode(func_node->left, key);
//     else if (key> func_node->value)
//         func_node->right = deleteNode(func_node->right, key);
//     else
//     {
//         //No child
//         if(func_node->right == NULL && func_node->left == NULL)
//         {
//             delete func_node;
//             func_node = NULL;   
//         }
//         //One child 
//         else if(func_node->right == NULL)
//         {
//             bstree* temp = func_node;
//             func_node= func_node->left;
//             delete temp;
//         }
//         else if(func_node->left == NULL)
//         {
//             bstree* temp = func_node;
//             func_node= func_node->right;
//             delete temp;
//         }
//         //two child
//         else
//         {
//             bstree* temp = FindMax(func_node->left);
//             func_node->value = temp->value;
//             func_node->left = deleteNode(func_node->left, temp->value);
//         }
//     }
//     return func_node;
// }

bstree *root = NULL;
pthread_mutex_t fine_lock;
pthread_rwlock_t rwlock;


/* Function name: minValueNode
 * Description: The function is used to find the node with the minimal value that is 
 *              that is useful while deleting the nodes recursively. It is called in the delete function.
 */

bstree* minValueNode(bstree* node){
   bstree* current = node;
   while (current && current->left != NULL)
      current = current->left;
   return current;
}

/* Function name: deleteNode
 * Description: The function is used to delete the nodes recursively based on the input node and key. If the node is
 *              not found, the function returns a NULL.
 */

bstree* deleteNode(bstree* func_node, int key)
{
//     cout << "1" << endl;
    if (func_node == NULL) 
    {
        return func_node;
    }
    
    if(hoh_flag) pthread_mutex_lock(&func_node->lock);
    if(rw_flag) pthread_rwlock_wrlock(&func_node->rwlock);
        
    if (key < func_node->key)
    {
//         cout << "4" << endl;
        
        func_node->left = deleteNode(func_node->left, key);
        
        if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
        if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);
        
//         cout << "5" << endl;
    }
    else if (key > func_node->key)
    {
//         cout << "6" << endl;
       
        func_node->right = deleteNode(func_node->right, key);
        
        if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
        if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);
        
//         cout << "7" << endl;
    }
    else
    {
        if (func_node->left == NULL)
        {
//             cout << "8" << endl;
            bstree *temp = func_node->right;
            free(func_node);
//             cout << "9" << endl;
            return temp;
        }
        else if (func_node->right == NULL)
        {
//             cout << "10" << endl;
            bstree* temp = func_node->left;
            free(func_node);
//             cout << "11" << endl;
            return temp;
        }
//         cout << "12" << endl;
        bstree* temp = minValueNode(func_node->right);
        func_node->key = temp->key;
        func_node->right = deleteNode(func_node->right, temp->key);
//         cout << "13" << endl;
    }
//     cout << "14" << endl;
    return func_node;
}


/* Function name: put
 * Description: The function is used to add the requested node onto the BST. It too is done recursively based on the value of the other
 *              keys and the nodes.
 */

bstree *put(bstree *func_node, int key, int value)
{
    if(func_node == NULL) 
    {
        if(hoh_flag) pthread_mutex_lock(&fine_lock);
        if(rw_flag) pthread_rwlock_wrlock(&rwlock);

        if(root == NULL) 
        {
            //if the root is NULL, create a new node with the requested key and value
            root = new_node(key, value);

            if(hoh_flag) pthread_mutex_unlock(&fine_lock);
            if(rw_flag) pthread_rwlock_unlock(&rwlock);

            return root;
        }

        if(hoh_flag) pthread_mutex_lock(&root->lock);
        if(rw_flag) pthread_rwlock_wrlock(&root->rwlock);

        func_node = root; 

        if(hoh_flag) pthread_mutex_unlock(&fine_lock);
        if(rw_flag) pthread_rwlock_unlock(&rwlock);
    }

    //If the key is lesser than that of BST key, start by recursing through the left part of the BST.
    if (key < func_node->key) 
    {	
        if(func_node->left == NULL) 
        {
            //If left most node is NULL, add a newer node.
            func_node->left = new_node(key, value); 

            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            return func_node->left;
        }
        else 
        {
            if(hoh_flag) pthread_mutex_lock(&func_node->left->lock);
            if(rw_flag) pthread_rwlock_wrlock(&func_node->left->rwlock);

            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            put(func_node->left, key, value); 
        }
    }
   
    //If the key is greater than that of BST key, start by recursing through the right part of the BST.
    else if (key > func_node->key) 
    {	
        if(func_node->right == NULL) 
        {
            //If left most node is NULL, add a newer node.
            func_node->right = new_node(key, value); //Insert new node

            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            return func_node->right;
        }
        else
        {
            if(hoh_flag) pthread_mutex_lock(&func_node->right->lock);
            if(rw_flag) pthread_rwlock_wrlock(&func_node->right->rwlock);

            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            put(func_node->right, key, value); 
        }    
    }
    
    //This indicates that the key already exists, do nothing.
    else if (key == func_node->key) 
    {
        if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
        if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);
    }

    
    return NULL; 
}

int bst_get(bstree *func_node, int key)
{
    bstree *temp = NULL;
    temp = get(func_node, key); 
    if(temp == NULL) return (-1);
    else return (temp->value); 
}

/* Function name: get
 * Description: The function is used to search for the requested node onto the BST. It too is done recursively based on the value of the other
 *              keys and the nodes.
 */

bstree *get(bstree *func_node, int key)
{ 
    if(func_node == NULL) 
    {
        if(hoh_flag) pthread_mutex_lock(&fine_lock);
        if(rw_flag) pthread_rwlock_rdlock(&rwlock);

        if(root == NULL) 
        {
             //if the root is NULL, return the same
            if(hoh_flag) pthread_mutex_unlock(&fine_lock);
            if(rw_flag) pthread_rwlock_unlock(&rwlock);

            return root;
        }

        if(hoh_flag) pthread_mutex_lock(&root->lock);
        if(rw_flag) pthread_rwlock_rdlock(&root->rwlock);

        func_node = root; 

        if(hoh_flag) pthread_mutex_unlock(&fine_lock);
        if(rw_flag) pthread_rwlock_unlock(&rwlock);
    }

     //If the key is lesser than that of BST key, start by recursing through the left part of the BST.
    if(key < func_node->key) 
    {
        if (func_node->left == NULL) 
        {
            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);
            return func_node->left;
        } 
        else 
        {
            if(hoh_flag) pthread_mutex_lock(&func_node->left->lock);
            if(rw_flag) pthread_rwlock_rdlock(&func_node->left->rwlock);

            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            return get(func_node->left, key);
        }
    }
     //If the key is greater than that of BST key, start by recursing through the right part of the BST.
    else if (key > func_node->key) 
    {
        if (func_node->right == NULL)
        {
            if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            return func_node->right;
        } 
        else 
        {
            if(hoh_flag) pthread_mutex_lock(&func_node->right->lock);
            if(rw_flag) pthread_rwlock_rdlock(&func_node->right->rwlock);

           if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
            if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

            return get(func_node->right, key);
        }
    } 
     //This indicates that the key already exists, return the node.
    else
    {
        if(hoh_flag) pthread_mutex_unlock(&func_node->lock);
        if(rw_flag) pthread_rwlock_unlock(&func_node->rwlock);

        return func_node;
    }

}


/* Function name: bst_range_query
 * Description: The function is used to print the values between the start and the end keys in the BST. 
 */

void bst_range_query(bstree* node, int start_key, int end_key)
{
    if (node == NULL)
    {
        //If the node is NULL, the root is checked to see NULL
        pthread_mutex_lock(&fine_lock);
        if (root == NULL)
        {
            pthread_mutex_unlock(&fine_lock);
            return;
        }

        pthread_mutex_lock(&root->lock);
        node = root;
        pthread_mutex_unlock(&fine_lock);
    }
    if ((start_key <= node->key) && (end_key >= node->key))
    {
       cout << "Key: " << node->key << " Corresponding Value: " << node->value << endl;
        
    }
    if (end_key > node->key)
    {
        if (node->right != NULL)
        {
            pthread_mutex_lock(&node->right->lock);
            pthread_mutex_unlock(&node->lock);
            bst_range_query(node->right, start_key, end_key);
            pthread_mutex_lock(&node->lock);
        } 
    }
    if (start_key < node->key)
    {
        if (node->left != NULL)
        {
            pthread_mutex_lock(&node->left->lock);
            pthread_mutex_unlock(&node->lock);
            bst_range_query(node->left, start_key, end_key);
            pthread_mutex_lock(&node->lock);
        } 
    }
    pthread_mutex_unlock(&node->lock);

}

