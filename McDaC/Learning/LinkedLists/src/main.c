#include "include/nodeFuncs.h"
#include "include/hashTable.h"

#include <stdio.h>

int main()
{
	initHashTable(10);

	node_t node1 = {.var1 = 3, .var2 = 9.4f}, 
		   node2 = {.var1 = 7, .var2 = 4.7f}, 
		   node3 = {.var1 = 5, .var2 = 0.1f};

	hashTableInsert(&node1);
	hashTableInsert(&node2);
	hashTableInsert(&node3);

	printHashTable();

	freeHashTable();

	/*
	node_t *root;
	initalizeRootNode(&root, 5, 9.4f);

	root->next = createNode();
	root->next->back = root;
	setNodeVal(root, 2, 7, 1.9f);

	insertNode(root, root->next);

	freeNodeEnd(root);

	printList(root);

	freeAllNodes(root);
	*/
	return 0;
}
