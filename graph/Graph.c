/*************************************************************************
    > File Name: Graph.c
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Fri 23 Oct 2015 08:39:37 CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"Graph.h"

graph* Init_graph(int nn)
{
	int i;
	if(nn >Nmax)return error;
	//if the size is too huge, return error!

	graph* G = (graph*)malloc(sizeof(graph));
	G ->list =(Vertex *)malloc((nn+3)*sizeof(Vertex));
	G ->head =(Index *)malloc((nn+3)*sizeof(Index));
	G ->pre =(Index **)malloc((nn+3)*sizeof(Index*));
	G ->dis =(int **)malloc((nn+3)*sizeof(int *));
	G ->info =(edge *)malloc(Mmax*sizeof(edge));

  for(i=0;i <G ->n;i++)
	{
		G ->head[i] =-1;
		G ->dis[i] =(int *)malloc((nn+3)*sizeof(int));
		G ->pre[i] =(int *)malloc((nn+3)*sizeof(int));
	}

	G ->n =nn;
	G ->m =0;
	return G;
}

status Graph_Destroy(graph* G)
{
	free(G->list);
	free(G->head);
	free(G->pre);
	free(G->dis);
	free(G->info);
	free(G);
	return ok;
}

status prim(graph* G,int index)
{
	if(!G ||!G ->info || !G ->head)return error;
	bool* visited;
	int i,j;
	int temp;
	int templen;
	visited =(bool *)malloc((G ->n+5)*sizeof(bool));

	for(i=0;i<G ->n;i++)
	{
		visited[i] =false;
		G ->dis[index][i] =INF;
		G ->pre[index][i] =index;
	}

	G ->dis[index][index] =0;
	visited[index] =true;
	for(i =G ->head[index];i !=-1;i =G ->info[i].next)
	{
		G ->pre[index][G ->info[i].destin] =index;
		G ->dis[index][G ->info[i].destin] =G ->info[i].len;
	}
	//init the prim algorithm!

	while(1)
	{
		temp =index;
		templen =INF;
		for(i =0; i <G ->n;i++)
		{
			if(G ->dis[index][i] <templen && !visited[i])
			{
				temp =i;
				templen =G ->dis[index][i];
			}
		}

		if(temp ==index)break;
		visited[temp] =true;
		int tifo =G ->dis[index][temp];

		for(i =G ->head[temp]; i != -1; i =G ->info[i].next)
		{
			int rev =G ->info[i].destin;
			if(G ->dis[index][rev] > tifo +G ->info[i].len)
			{
				  G ->pre[index][rev] =temp;
			      G ->dis[temp][rev] = tifo +G ->info[i].len;
			}
		}
   }
	return ok;
}


status addEdge(graph* G,int from,int to,int lenn)
{
	if(!G ||!G ->info || !G ->head)return error;
	if(G ->m >Mmax)return error;
	int i =G ->m;
	G ->info[i].destin =to;
	G ->info[i].len =lenn;
	G ->info[i].next =G ->head[from];
	G ->head[from] =G ->m++;
	return ok;
}

Index LocateVer(graph *G,char info[])
{
	if(!G ||!G ->info || !G ->head)return -1;
	int i;
	for(i=0;i <G ->n;i++)
	{
		if(strcmp(G ->list[i].name,info)==0)return i;
	}
	return -1;
}

status Query(graph* G,char a[],char b[])
{
	int c =LocateVer(G,a);
	int d =LocateVer(G,b);
	if(c ==-1||d ==-1)return error;
	int i =G ->pre[c][d],temp[201],j;

	temp[0] =d;
	j =1;
	while(i!=c)
	{
		temp[j++] =i;
		i =G ->pre[c][i];
	}
	temp[j] =c;
	for(--j;j>=0;j--)
		printf("From %s to %s and walks %d M\n",G ->list[temp[j+1]].name,G ->list[temp[j]].name,G ->dis[temp[j+1]][temp[j]]);

	return ok;
}

status Introduce(graph *G,char info[])
{
	int c =LocateVer(G,info);
	if(c ==-1)return error;
	printf("%s",G ->list[c].info);
	return ok;
}

status Update(graph *G)
{
	FILE* fp;
	fp =fopen("src","r");

	fscanf(fp,"%d",&G ->n);
	fgetc(fp);
	G = Init_graph(G ->n);

	int i,j;
	for(i=0;i<G ->n;i++)
	{
		fscanf(fp,"%s",G ->list[i].name);
		fgetc(fp);
		fgets(G ->list[i].info,255,fp);
	}

	int a,b,c;
	while(fscanf(fp,"%d %d %d",&a,&b,&c)!=EOF)
	{
		addEdge(G,a,b,c);
		addEdge(G,b,a,c);
	}
	fclose(fp);

	for(i=0;i<G ->n;i++)prim(G,i);

	fp =fopen("data","w");
	fprintf(fp,"%d\n",G ->n);
	for(i=0;i<G ->n;i++)
	{
		fprintf(fp,"%s\n",G ->list[i].name);
		fprintf(fp,"%s\n",G ->list[i].info);
	}
	for(i=0;i<G ->n;i++)
	{
		for(j=0;j<G ->n;j++)
			fprintf(fp,"%d ",G ->dis[i][j]);
        fprintf(fp,"\n");
	}

	for(i=0;i<G ->n;i++)
	{
		for(j=0;j<G ->n;j++)
			fprintf(fp,"%d ",G ->pre[i][j]);
        fprintf(fp,"\n");
	}
	fclose(fp);
	return ok;
}


status Read(graph *G)
{
	FILE* fp;
	fp =fopen("data","r");
	if(!fp)return error;

	fscanf(fp,"%d",&G ->n);
	getc(fp);
	G = Init_graph(G ->n);

	int i,j;
	for(i=0;i<G ->n;i++)
	{
		fscanf(fp,"%s",G ->list[i].name);
		getc(fp);
		fgets(G ->list[i].info,255,fp);
	}
	for(i=0;i<G ->n;i++)
	{
		for(j=0;j<G ->n;j++)
			fscanf(fp,"%d",&G ->dis[i][j]);
	}

	for(i=0;i<G ->n;i++)
	{
		for(j=0;j<G ->n;j++)
			fscanf(fp,"%d",&G ->pre[i][j]);
	}
	fclose(fp);
	return ok;
}




