/*************************************************************************
    > File Name: Huffman.h
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Fri 09 Oct 2015 20:06:48 CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#define HEAP_MAX 100

typedef enum _STATUS
{
	ok,
	error,
}status;

typedef struct Huffman_leave
{
	int key;
	char c;
	struct Huffman_leave* lchild;
	struct Huffman_leave* rchild;
}leaf;

typedef struct Huffman_tree
{
	struct Huffman_leave* info;
	struct Huffman_leave* root;
}HuffmanTree;

typedef struct _HEAP
{
	int size;
	int capacity;
	struct Huffman_leave** data;
}heap;

status init_heap(heap* root);
status min_heapify(heap* root,int index);
status heap_insert(heap* root,leaf* e);
leaf* heap_pop(heap* root);
int min(int a,int b);
status Huffman_coding(leaf* root,char* code);
status Huffman_translate(leaf* T);
status InitTree(HuffmanTree* T,int n);
