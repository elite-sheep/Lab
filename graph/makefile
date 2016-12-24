all:CompusMap

Graph:
	gcc Graph.h Graph.c -o Graph.o

main:
	gcc main.c -o main.o

CompusMap:Graph.o main.o
	gcc Graph.o main.o -o CompusMap 
