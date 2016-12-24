all:Huffman_tree

Huffman:
	gcc -g Huffman.h Huffman.c -o Huffman.o

test:
	gcc -g test.c -o test.o

Huffman_tree:Huffman.o test.o
	gcc -g Huffman.o test.o -o Huffman_tree
