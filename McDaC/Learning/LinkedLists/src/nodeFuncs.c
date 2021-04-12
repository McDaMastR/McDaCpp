#include "include/nodeFuncs.h"

#include <stdlib.h>
#include <stdio.h>

node_t *createNode()
{
	node_t *node = (node_t *) malloc(sizeof(node_t));
	node->next = NULL;
	node->back = NULL;
	return node;
}

void initalizeNode(node_t **new_node, const int var1, const float var2)
{
	*new_node = (node_t *) malloc(sizeof(node_t));
	(*new_node)->next = NULL;
	(*new_node)->back = NULL;
	(*new_node)->var1 = var1;
	(*new_node)->var2 = var2;
}

void insertNode(node_t * const previous_node, node_t * const next_node)
{
	node_t * const node = (node_t *) malloc(sizeof(node_t));

	node->back = next_node->back;
	node->next = previous_node->next;

	next_node->back = node;
	previous_node->next = node;
}

void insertNodeEnd(node_t * const previous_node)
{	
	node_t *node = (node_t *) malloc(sizeof(node_t));

	previous_node->next = node;
	node->back = previous_node;
}

void freeNode(node_t * const node)
{
	if (node->back)
		node->back->next = node->next;
	if (node->next)
		node->next->back = node->back;
	free(node);
}

void freeNodeEnd(node_t * const node)
{
	node_t * const end_node = getLastNode(node);
	end_node->back->next = NULL;
	free(end_node);
}

void freeAllNodes(node_t * const root)
{
	free(root);
	if (root->next) {
		freeAllNodes(root->next);
	}
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
