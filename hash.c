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

int checkUndeclaredHash() {
    int undeclared = 0;
    for (int i = 0; i < SIZE; i++) {
        for (struct hashNode* node = hashTable[i]; node; node = node->next) {
            if (node->tokenType == SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Semantic error: identifier %s wasn't declared.\n", node->text);
                undeclared++;
            }
        }
    }
    return undeclared;
}

struct hashNode* makeTempHash() {
    static int serialNum = 0;
    char buffer[256] = "";

    /* It is important that the text of this temporary
     * is unique. To avoid any conflict with user-
     * defined nodes, we give it a bizarre name. */
    sprintf(buffer, "TtE_eMm_EPp%d", serialNum++);
    return insertHash(SYMBOL_VARIABLE, buffer);
}

struct hashNode* makeLabelHash() {
    static int serialNum = 0;
    char buffer[256] = "";

    /* Same as the makeTempHash() function, this node's
     * text must be unique, so we give it a weird name. */
    sprintf(buffer, "lLa_bBe_Ll1_%d", serialNum++);
    return insertHash(SYMBOL_LABEL, buffer);
}

void printAsmTempsAndLiterals(FILE* out) {
    for (int i = 0; i < SIZE; i++) {
        for (struct hashNode* node = hashTable[i]; node; node = node->next) {
            if (node->text == strstr(node->text, "TtE_eMm_EPp") || node->tokenType == SYMBOL_FUNCTION) {
                fprintf(out,    "\t.bss\n"
                                "\t.globl\t_%s\n"
                                "\t.type\t_%s, @object\n"
                                "\t.size\t_%s, 4\n"
                                "_%s:\n"
                                "\t.zero\t4\n",
                        node->text, node->text, node->text, node->text);
            }
            else if (node->tokenType == SYMBOL_LIT_INTE) {
                fprintf(out,    "\t.section\t.rodata\n"
                                "\t.globl\t_%s\n"
                                "\t.type\t_%s, @object\n"
                                "\t.size\t_%s, 4\n"
                                "_%s:\n"
                                "\t.long\t%s\n",
                        node->text, node->text, node->text, node->text, node->text);
            }
            else if (node->tokenType == SYMBOL_LIT_REAL) {
                fprintf(out,    "\t.section\t.rodata\n"
                                "\t.globl\t_%s\n"
                                "\t.type\t_%s, @object\n"
                                "\t.size\t_%s, 4\n"
                                "_%s:\n"
                                "\t.float\t%s\n",
                        node->text, node->text, node->text, node->text, node->text);
            }
            else if (node->tokenType == SYMBOL_LIT_CARA) {
                fprintf(out,    "\t.section\t.rodata\n"
                                "\t.globl\t_%s\n"
                                "\t.type\t_%s, @object\n"
                                "\t.size\t_%s, 4\n"
                                "_%s:\n"
                                "\t.long\t%d\n",
                        node->text, node->text, node->text, node->text, node->text[1]);
            }
        }
    }
}
