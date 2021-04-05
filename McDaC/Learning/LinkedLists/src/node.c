#include "include/node.h"

#include <stdlib.h>
#include <assert.h>

node_t *createRootNode()
{
	node_t *root = malloc(sizeof(node_t));
	root->next = NULL;
	root->back = NULL;
	return root;
}

node_t *createNode()
{
	node_t *temp = malloc(sizeof(node_t));
	temp->next = NULL;
	temp->back = NULL;
	return temp;
}

void initalizeRootNode(node_t **root, const int var1, const float var2)
{
	*root = malloc(sizeof(node_t));
	(*root)->next = NULL;
	(*root)->back = NULL;
	(*root)->var1 = var1;
	(*root)->var2 = var2;
}

void initalizeNode(node_t **new_node, node_t * const previous_node, const int var1, const float var2)
{
	*new_node = malloc(sizeof(node_t));
	(*new_node)->next = NULL;
	(*new_node)->back = previous_node;
	(*new_node)->var1 = var1;
	(*new_node)->var2 = var2;
}

void insertNode(node_t * const * const previous_node, node_t * const * const next_node)
{
	node_t * const node = malloc(sizeof(node_t));

	node->back = *previous_node;
	node->next = *next_node;

	(*previous_node)->next = node;
	(*next_node)->back = node;
}

void insertNodeEnd(node_t * const * const previous_node)
{	
	node_t *node = malloc(sizeof(node_t));

	(*previous_node)->next = node;
	node->back = *previous_node;
}

void freeNode(node_t * const node)
{
	node->back->next = node->next;
	node->next->back = node->back;
	free(node);
}

void freeNodeEnd(node_t * const node)
{
	node_t * const end_node = getLastNode(node);
	// DEBUG_LOG("%p\n", node);
	// DEBUG_LOG("%p\n", node->next->next->back);
	// DEBUG_LOG("%p\n", end_node->back);
	end_node->back->next = NULL;
	free(end_node);
}

void freeAllNodes(node_t * const root)
{
	free(root);
	if (root->next)
		freeAllNodes(root->next);
}

void setNodeVal(node_t * const root, const int node_num, const int var1, const float var2)
{
	node_t * const node = getNode(root, node_num);
	node->var1 = var1;
	node->var2 = var2;
}

void setNodeVar1(node_t * const root, const int node_num, const int var1)
{
	node_t * const node = getNode(root, node_num);
	node->var1 = var1;
}

void setNodeVar2(node_t * const root, const int node_num, const float var2)
{
	node_t * const node = getNode(root, node_num);
	node->var2 = var2;
}

node_t *getNode(node_t * const root, const int node_num)
{
	node_t *node = root;
	for (int i = 1; i < node_num; i++)
		node = node->next;
	return node;
}

node_t *getLastNode(node_t * const node)
{
	node_t *last_node;
	for (last_node = node; last_node->next; last_node = last_node->next) {}
	return last_node;
}

node_t *findNodeVar1(node_t * const root, const int	var1)
{
	node_t *node;
	for (node = root; node; node = node->next)
		if (node->var1 == var1)
			return node;
	return NULL;
}

node_t *findNodeVar2(node_t * const root, const float var2)
{
	node_t *node;
	for (node = root; node; node = node->next)
		if (node->var2 == var2)
			return node;
	return NULL;
}

void printList(node_t * const root)
{
	node_t *node;
	for (node = root; node; node = node->next) {
		printNode(node);
	}
}

void printNode(const node_t * const node)
{
	printf("var1 = %d\nvar2 = %f\n", node->var1, node->var2);
}
