#include "include/node.h"

#include <assert.h>

/*

To Compile:

time clang -g -Og -DDEBUG src/main.c src/node.c -o bin/LinkedLists

*/

int main()
{
	node_t *root;
	initalizeRootNode(&root, 5, 9.4f);

	root->next = createNode();
	root->next->back = root;
	setNodeVal(root, 2, 7, 1.9f);

	insertNode(&root, &root->next);

	DEBUG_LOG("%p\n", root);
	DEBUG_LOG("%p\n", root->next->back);
	DEBUG_LOG("%p\n", root->next->next->back->back);

	freeNodeEnd(root);


	// printList(root);

	freeAllNodes(root);
	return 0;
}
