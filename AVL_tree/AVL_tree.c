/*************************************************************************
    > File Name: AVL_tree.c
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: 2015年09月29日 星期二 22时45分29秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"AVL_tree.h"

int Max(int a, int b)
{
	if(a > b)return a;
	else return b;
}
status init_tree(AVL_tree* T, int e)
{
	T ->root = (AVL_leaf *)malloc(sizeof(AVL_leaf));
	T ->nil = (AVL_leaf *)malloc(sizeof(AVL_leaf));
	//if allocate failed, return error
	if(!T || !T ->root || !T ->nil)return error;

	T ->root ->lchild = T ->root ->rchild = T ->nil;
	T ->root ->data = e;
	T ->nil ->height = 0;
	T ->root ->height = 1;
	return ok;
}


status l_rotate(AVL_tree* root, AVL_leaf* x)
{
	//hints:after rotate,the inorder of the tree will not change;

	AVL_leaf* temp = x ->rchild;
	if(temp == NULL)return error;
	//change the lchild of y to the x
	x ->rchild = temp ->lchild;
	if(temp ->lchild != NULL)
		temp ->lchild ->parent = x;
	temp ->parent = x ->parent;
	temp ->lchild = x;
	x ->parent = temp;

	x ->height = Max(x ->lchild ->height,x ->rchild ->height)+1;
	temp ->height = Max(temp ->lchild ->height, temp ->rchild ->height)+1;
	return ok;
}

status r_rotate(AVL_tree* root, AVL_leaf* x)
{
	//hints:after rotate,the inorder of the tree will not change;

	AVL_leaf* temp = x ->lchild;
	if(temp == NULL)return error;
	//change the rchild of y to the x
	x ->lchild = temp ->rchild;
	if(temp ->rchild != NULL)
		temp ->rchild ->parent = x;
	temp ->parent = x ->parent;
	temp ->rchild = x;
	x ->parent = temp;
	x ->height = Max(x ->lchild ->height,x ->rchild ->height)+1;
	temp ->height = Max(temp ->lchild ->height, temp ->rchild ->height)+1;
	return ok;
}

status AVL_insert(AVL_tree* T, AVL_leaf *root, int e)
{
	if(e <= root ->data)
	{
		if(root ->lchild == T ->nil)
		{
		    root ->lchild = (AVL_leaf *)malloc(sizeof(AVL_leaf));
			root ->lchild ->lchild = root ->lchild ->rchild = T ->nil;
			root ->lchild ->data = e;
			root ->lchild ->parent = root;
			root ->lchild ->height = 1;
		}
		else AVL_insert(T,root ->lchild,e);
		AVL_leaf* pa = root ->lchild;
		AVL_leaf* pb = root ->rchild;

		if(pa ->height - pb ->height == 2)
		{
			if(pa ->lchild ->height > pa ->rchild ->height)
			    r_rotate(T,root);
			else
			{
				l_rotate(T,pa);
				r_rotate(T,root);
			}
			return ok;
		}
		root ->height = Max(root ->lchild ->height, root ->rchild ->height)+1;
	}
	else
	{
    	if(root ->rchild == T ->nil)
		{
            root ->rchild = (AVL_leaf *)malloc(sizeof(AVL_leaf));
			root ->rchild ->lchild = root ->rchild ->rchild = T ->nil;
			root ->rchild ->parent = root;
			root ->rchild ->data = e;
			root ->rchild ->height = 1;
		}
		else AVL_insert(T,root ->rchild,e);
		AVL_leaf* pa = root ->lchild;
		AVL_leaf* pb = root ->rchild;
		if(pb ->height - pa ->height == 2)
		{
			if(pb ->lchild ->height < pb ->rchild ->height)
			    l_rotate(T,root);
			else
			{
				r_rotate(T,pb);
				l_rotate(T,root);
			}
			return ok;
		}
		root ->height = Max(root ->lchild ->height, root ->rchild ->height)+1;
	}
	return ok;
}

status AVL_delete(AVL_tree* T, AVL_leaf *root, int e)
{
	AVL_leaf* pa = root ->lchild;
	AVL_leaf* pb = root ->rchild;
	if(e <= root ->data)
	{
		if(pa == T ->nil)
		{
		    return error;
		}
		else
			AVL_delete(T,root ->lchild,e);
		if(pb ->height - pa ->height == 2)
		{
			if(pb ->rchild ->height > pb ->lchild ->height)
			    l_rotate(T,root);
			else
			{
				r_rotate(T,pb);
				l_rotate(T,root);
			}
			return ok;
		}
		root ->height =Max(pa ->height,pb ->height);
	}
	else
	{
    	if(pb == T ->nil)
		{
			return error;
		}
		else AVL_delete(T,pb,e);
		if(pa ->height - pb ->height == 2)
		{
			if(pa ->lchild ->height > pa ->rchild ->height)
			    r_rotate(T,root);
			else
			{
				l_rotate(T,pa);
				r_rotate(T,root);
			}
			return ok;
		}
		root ->height = Max(pa ->height, pb ->height);
	}
	return ok;
}



