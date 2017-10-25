/*************************************************************************
    > File Name: Graph.h
    > Author: Yuchen Wang
    > Mail: wyc8094@gmail.com 
    > Created Time: Fri 23 Oct 2015 08:25:22 CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define Nmax 200
#define Mmax 20000
#define INF 0x3f3f3f3f
//define the max size of a graph to prevent overflow!

typedef int Index;

typedef enum _STATUS
{
	error,
	ok,
}status;

typedef struct _VERTEX
{
	char name[256];
	char info[256];//allow to introduce the place within 255 wo
}Vertex;

typedef struct _EDGE
{
	int destin;
	int len;
	Index next;
}edge;

typedef struct _GRAPH
{
	Vertex* list;
	Index** pre;
	struct _EDGE* info;
	Index* head;
	int** dis; //mark the min distance between two vertex
	int n;//The number of vetex
	int m;//The number of edge
}graph;

graph* Init_graph(int nn);
status Graph_Destroy(graph* G);
status prim(graph* G,int index);
status addEdge(graph* G,int from,int to,int len);
Index LocateVer(graph *G,char info[]);
status Query(graph* G,char a[],char b[]);
status Update(graph *G);
status Introduce(graph *G,char info[]);
status Read(graph *G);

