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

int hashFunction(char* text) {
    unsigned long value = 7;
    for (int i = 0; text[i]; i++) {
        value = value * 31 + text[i];
    }

    return value % SIZE;
}

struct hashNode* searchHash(char* text) {
    int index = hashFunction(text);
    for (struct hashNode* node = hashTable[index]; node; node = node->next) {
        if (strcmp(node->text, text) == 0) {
            return node;
        }
    }

    return NULL;
}

struct hashNode* insertHash(int tokenType, char* text) {
    struct hashNode* node = searchHash(text);
    if (node != NULL) {
        return node;
    }

    node = (struct hashNode*) calloc(1, sizeof(struct hashNode));
	node->tokenType = tokenType;
	node->text = (char*) calloc(strlen(text) + 1, sizeof(char));
	strcpy(node->text, text);

    int index = hashFunction(text);
	node->next = hashTable[index];
	hashTable[index] = node;

	return node;

}

void printHash() {
    for (int i = 0; i < SIZE; i++) {
        int j = 0;
        for (struct hashNode* node = hashTable[i]; node; node = node->next) {
            printf("hashTable[%d][%d] - (%d: %s)\n", i, j, node->tokenType, node->text);
            j++;
        }
    }
}
