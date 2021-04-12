#include "include/hashTable.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define BIT_SHIFT(x) 1 << x

static int g_count;
static node_t **g_hashTable;

unsigned int hash(int var1)
{
    var1 = ((var1 >> 16) ^ var1) * 0x45d9f3b;
    var1 = ((var1 >> 16) ^ var1) * 0x45d9f3b;
    var1 = (var1 >> 16) ^ var1;
    return var1 % g_count;
}

void initHashTable(const int count)
{
    srand(time(NULL));
    g_count = count;
    g_hashTable = calloc(count, sizeof(node_t *));
}

void freeHashTable()
{
    free(g_hashTable);
}

bool hashTableInsert(node_t *node)
{
    if (!node)
        return false;
    unsigned int index = hash(node->var1);
    if (g_hashTable[index])
        return false;
    g_hashTable[index] = node;
    return true;
}

void printHashTable()
{
    for (short i = 0; i < g_count; i++) {
        printf("%d:\t", i);
        if (g_hashTable[i])
            printf("Var1: %d\tVar2: %f\n", g_hashTable[i]->var1, g_hashTable[i]->var2);
        else
            printf("Var1: -\tVar2: -\n");
    }
    printf("\n");
}
