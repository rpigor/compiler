#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashNode* hashTable[SIZE];

void initHash() {
    for (int i = 0; i < SIZE; i++) {
        hashTable[i] = NULL;
    }
}

int hashFunction(char* symbol) {
    unsigned long value = 7;
    for (int i = 0; symbol[i]; i++) {
        value = value * 31 + symbol[i];
    }

    return value % SIZE;
}

struct hashNode* searchHash(char* symbol) {
    int index = hashFunction(symbol);
    for (struct hashNode* node = hashTable[index]; node; node = node->next) {
        if (strcmp(node->symbol, symbol) == 0) {
            return node;
        }
    }

    return NULL;
}

struct hashNode* insertHash(int tokenType, char* symbol) {
    struct hashNode* node = searchHash(symbol);
    if (node != NULL) {
        return node;
    }

    node = (struct hashNode*) calloc(1, sizeof(struct hashNode));
	node->tokenType = tokenType;
	node->symbol = (char*) calloc(strlen(symbol) + 1, sizeof(char));
	strcpy(node->symbol, symbol);

    int index = hashFunction(symbol);
	node->next = hashTable[index];
	hashTable[index] = node;

	return node;

}

void printHash() {
    for (int i = 0; i < SIZE; i++) {
        int j = 0;
        for (struct hashNode* node = hashTable[i]; node; node = node->next) {
            printf("hashTable[%d][%d] - (%d: %s)\n", i, j, node->tokenType, node->symbol);
            j++;
        }
    }
}
