/*************************************************************************
    > File Name: test.c
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Sun 18 Oct 2015 16:47:28 CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include"Huffman.h"
#include<stdlib.h>

int main()
{
	int i;
	heap test;
	init_heap(&test);
	freopen("input","r",stdin);
	leaf *temp1;
	leaf *temp2;
	HuffmanTree T;
	int num;
	scanf("%d",&num);
	InitTree(&T,num);
	getchar();
	for(i=0;i<num;i++)
	{
		scanf("%c %d",&T.info[i].c,&T.info[i].key);
		T.info[i].lchild = T.info[i].rchild =NULL;
		//initalize the child pointers
		heap_insert(&test,&T.info[i]);
		getchar();
	}
	while(1)
	{
	    if(test.size ==1)break;
		temp1 =heap_pop(&test);
		temp2 =heap_pop(&test);
		T.info[i].key =temp1 ->key + temp2 ->key;
		T.info[i].lchild =temp1;
		T.info[i].rchild =temp2;
		heap_insert(&test,&T.info[i++]);
	}
	T.root =heap_pop(&test);
	freopen("coding","w",stdout);
	//output the coding list
	Huffman_coding(T.root,"0");
	fclose(stdout);
	freopen("codefile","r",stdin);
	freopen("textfile","w",stdout);
	Huffman_translate(T.root);
	return 0;
}


	



