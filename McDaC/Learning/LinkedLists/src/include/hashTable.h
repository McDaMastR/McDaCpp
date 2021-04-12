#pragma once

#include "node.h"

#include <stdbool.h>

// Hash to location in linked list
unsigned int hash(int var1);

// Initialize Hash Table
void initHashTable(const int count);

// Delete Hash Table
void freeHashTable();

// Insert Node into Hash Table
bool hashTableInsert(node_t *node);

// Create node in hash table
bool hashTableEmplace(const int var1, const float var2);

// Print Hash Table in Console
void printHashTable();
