all:AVL_tree

AVL_tree.o:
	gcc -c AVL_tree.c -o AVL_tree.o

test_AVL.o:
	gcc -c test_AVL.c -o test_AVL.o

AVL_tree:AVL_tree.o test_AVL.o
	gcc AVL_tree.o test_AVL.o -o AVL_tree

clean:
	rm *.o
