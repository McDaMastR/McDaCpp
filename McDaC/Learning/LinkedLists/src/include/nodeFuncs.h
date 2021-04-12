#pragma once

#include "node.h"

// Create Node
node_t *createNode();

// Create Node with initalized values
void initalizeNode 	  (node_t **new_node, const int var1, const float var2);

// Create and insert Node into list
void insertNode	  (node_t * const previous_node, node_t * const next_node);
void insertNodeEnd(node_t * const previous_node							 		 );

// Free node/s
void freeNode	 (node_t * const node);
void freeNodeEnd (node_t * const node);
void freeAllNodes(node_t * const root);

// Set value/s of a node
void setNodeVal (node_t * const root, const int node_num, const int var1, const float var2);
void setNodeVar1(node_t * const root, const int node_num, const int var1				  );
void setNodeVar2(node_t * const root, const int node_num, const float var2				  );

// Get Node with how deep the Node is
node_t *getNode	   (node_t * const root, const int node_num);
node_t *getLastNode(node_t * const node					   );

// Get Node with it's value
node_t *findNodeVar1(node_t * const root, const int	var1  );
node_t *findNodeVar2(node_t * const root, const float var2);

// Print Node/s in console
void printList(node_t * const root		);
void printNode(const node_t * const node);
