/*************************************************************************
    > File Name: main.c
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Sat 31 Oct 2015 16:35:12 CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

int main()
{
	graph G;
	printf("Welcome to this System\n");
	char Command;
	if(Read(&G)==error)
	{
		printf("Can not find data file!!\n");
	}
	while(1)
	{
		scanf("%c",&Command);
		if(Command =='q')break;
		else if(Command =='U')
		{
			Update(&G);
			printf("Update successfully\n");
		}
		else if(Command =='I')
		{
			char a[255];
			printf("Please input the name of the place\n");
			scanf("%s",a);
			getchar();
			if(Introduce(&G,a)==error)
			{
				printf("Bad index\n");
			}
		}
		else if(Command =='A')
		{
			char a[255],b[255];
			printf("Please input 2 places\n");
			scanf("%s",a);
			getchar();
			scanf("%s",b);
			getchar();
			if(Query(&G,a,b) ==error)
			{
				printf("Bad index\n");
			}
		}
	}
	return 0;
}





