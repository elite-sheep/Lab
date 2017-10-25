/*************************************************************************
    > File Name: Huffman.c
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Fri 09 Oct 2015 20:15:16 CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Huffman.h"

int min(int a,int b)
{
	if(a<b)return a;
	return b;
}

heap* init_heap()
{
	heap* root = (heap*)malloc(sizeof(heap));
	root ->data =(leaf**)malloc(HEAP_MAX*sizeof(leaf*));
	if(!root ->data)return error;
	//if allocation failed,return error

	root ->capacity = HEAP_MAX;
	root ->size = 0;
	return ok;
}

status Destroy_heap(heap* root)
{
	free(root->data);
	free(root);
	return ok;
}

status min_heapify(heap* root,int index)
{
	//The index and the data shpuld follow the rules!
	if(!root||!root ->data)return error;
	if(index<=0||index>root ->size)return error;
	leaf* temp;
	int largest =index;
	if(2*index<=root ->size)
	{
		if(root ->data[2*index] ->key <root ->data[largest] ->key)
			largest =2*index;
	}
	if(2*index+1 <=root ->size)
	{
	    if(root ->data[2*index+1] ->key <root ->data[largest] ->key)
			largest =2*index+1;
	}
	if(largest !=index)
    {
        temp =root ->data[largest];
        root ->data[largest] =root ->data[index];
        root ->data[index] =temp;
        min_heapify(root,largest);
    }
	return ok;
}
status heap_insert(heap* root,leaf* e)
{
	if(!root || !root ->data)return error;
	if(root ->size >= root ->capacity)return error;
	root ->data[++root ->size] = e;
	int i;

	for(i = root ->size;i > 0; i/=2)
		min_heapify(root,i);

	return ok;
}

leaf* heap_pop(heap* root)
{
	leaf* e;
	if(!root || !root ->data)return NULL;
	e = root ->data[1];
	root ->data[1] = root ->data[root ->size];
	root ->size--;
	if(root ->size >0)min_heapify(root,1);
	return e;
}


status Huffman_coding(leaf* root,char code[])
{
	if(!root)return error;
	char newcodea[200],newcodeb[200];
	int length;
	length =strlen(code);

	int flag; //to mark visit order!
	flag =0;

	if(root ->lchild !=NULL)
	{
		strcpy(newcodea,code);
		newcodea[length] ='0';
		newcodea[length+1] ='\0';
		Huffman_coding(root ->lchild,newcodea);
	}
	else flag++;

	if(root ->rchild !=NULL)
	{
		strcpy(newcodeb,code);
		newcodeb[length] ='1';
		newcodeb[length+1] ='\0';
		Huffman_coding(root ->rchild,newcodeb);
	}
	else flag++;

	if(flag ==2)printf("%c %s\n",root ->c,code+1);
	return ok;
}

status Huffman_translate(leaf* T)
{
	if(!T)return error;
	leaf* p;
	p = T;
	char code;
	while(scanf("%c",&code)!=EOF)
	{
		if(code =='0')
		{
			if(p ->lchild !=NULL)
			{
				p =p ->lchild;
				continue;
			}
			else
			{
                printf("%c",p ->c);
				p =T;
				continue;
			}
		}
		else if(code =='1')
		{
            if(p ->rchild !=NULL)
			{
				p =p->rchild;
				continue;
			}
			else
			{
                printf("%c",p ->c);
				p =T;
				continue;
			}
		}
	}
	return ok;
}


HuffmanTree* InitTree(int n)
{
	HuffmanTree* T = (HuffmanTree*)malloc(sizeof(HuffmanTree));
	T ->info =(leaf *)malloc((2*n+5)*sizeof(leaf));
	if(!T ->info)return error;
	T ->root =NULL;
	return ok;
}

status Destroy_HuffmanTree(HuffmanTree* T)
{
	Destroy_HuffmanLeave(T->root);
	Destroy_HuffmanLeave(T->info);
}

status Destroy_HuffmanLeave(leaf* T)
{
	if(T->lchild != NULL)
	{
		Destroy_HuffmanLeave(T->lchild);
	}
	if(T->rchild != NULL)
	{
		Destroy_HuffmanLeave(T->rchild);
	}
	free(T);
	return ok;
}



