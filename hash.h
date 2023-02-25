#pragma once

#define SIZE 547

struct hashNode {
    int tokenType;
    char* text;
    struct hashNode* next;
};

void initHash();
int hashFunction(char* text);
struct hashNode* searchHash(char* text);
struct hashNode* insertHash(int tokenType, char* text);
void printHash();
