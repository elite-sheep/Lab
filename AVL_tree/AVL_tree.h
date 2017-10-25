/*************************************************************************
    > File Name: AVL_tree.h
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: 2015年09月29日 星期二 22时18分39秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define ok true
#define error false

int Max(int a,int b);

typedef struct _AVL_leaf
{
	int data;
	struct _AVL_leaf* lchild;
	struct _AVL_leaf* rchild;
	struct _AVL_leaf* parent;
	int height;
}AVL_leaf;

typedef struct _AVL_tree
{
	struct _AVL_leaf* root;
	struct _AVL_leaf* nil;
}AVL_tree;

typedef bool status;

AVL_tree* init_tree(int e);
status AVL_Destroy(AVL_tree* T);
status AVL_Leaf_Destroy(AVL_leaf* T, AVL_leaf* nil);
status l_rotate(AVL_tree* T, AVL_leaf* x);
status r_rotate(AVL_tree* T, AVL_leaf* x);
status AVL_insert(AVL_tree* T,AVL_leaf* root, int e);
status AVL_delete(AVL_tree* T,AVL_leaf* root, int e);





