#pragma once

#include <stdio.h>

#define SIZE 547

enum hashDataTypes {
    DATATYPE_CARA,
    DATATYPE_INTE,
    DATATYPE_REAL
};

enum hashSymbols {
    SYMBOL_IDENTIFIER,
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_VECTOR,
    SYMBOL_LIT_INTE,
    SYMBOL_LIT_REAL,
    SYMBOL_LIT_CARA,
    SYMBOL_LIT_STRING,
    SYMBOL_LABEL
};

struct hashNode {
    int tokenType;
    int dataType;
    char* text;
    struct hashNode* next;
};

void initHash();
int hashFunction(char* text);
struct hashNode* searchHash(char* text);
struct hashNode* insertHash(int tokenType, char* text);
void printHash();
int checkUndeclaredHash();
struct hashNode* makeTempHash();
struct hashNode* makeLabelHash();
void printAsmTempsAndLiterals(FILE* out);
