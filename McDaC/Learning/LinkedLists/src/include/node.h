#pragma once

// Node
typedef struct node
{
	int var1;
	float var2;

	struct node *next;
	struct node *back;
} node_t;
