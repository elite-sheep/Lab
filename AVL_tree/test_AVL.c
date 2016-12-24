/*************************************************************************
    > File Name: test_AVL.c
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: 2015年10月01日 星期四 00时27分10秒
 ************************************************************************/

#include<stdio.h>
#include"AVL_tree.h"
int main()
{
	int a = 41;
	AVL_tree mytree;
	init_tree(&mytree,a);
	while(1)
	{
	    scanf("%d",&a);
		AVL_insert(&mytree,mytree.root,a);
		if(a == 0)break;
	}
		printf("The height of my AVL tree is %d",mytree.root ->height);
	return 0;
}
