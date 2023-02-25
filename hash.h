#pragma once

#define SIZE 547

struct hashNode {
    int tokenType;
    char* symbol;
    struct hashNode *next;
};

void initHash();
int hashFunction(char* symbol);
struct hashNode* searchHash(char* symbol);
struct hashNode* insertHash(int tokenType, char* symbol);
void printHash();
