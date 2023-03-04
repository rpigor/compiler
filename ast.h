#pragma once

#include "hash.h"
#include <stdio.h>

#define MAX_CHILDREN 4

enum astTypes {
    AST_DECL = 0,
    AST_TAIL,
    AST_DEC_VAR,
    AST_DEC_FUN,
    AST_DEC_VEC,
    AST_CARA,
    AST_INTE,
    AST_REAL,
    AST_LITSYMBOL,
    AST_SYMBOL,
    AST_VECINITL,
    AST_PARAML,
    AST_PARAM,
    AST_BLOCK,
    AST_CMDL,
    AST_ASSIGN,
    AST_VEC_ASSIGN,
    AST_ESCREVA,
    AST_SE,
    AST_SE_SENAUM,
    AST_ENQUANTO,
    AST_RETORNE,
    AST_VEC_ELEM,
    AST_ADD,
    AST_SUB,
    AST_DIV,
    AST_MUL,
    AST_LT,
    AST_GT,
    AST_DIF,
    AST_EQ,
    AST_GE,
    AST_LE,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_BRAC_EXPR,
    AST_FUNC_CALL,
    AST_ENTRADA,
    AST_EXPRL,
    AST_ARGL,
    AST_PRINTL
};

struct astNode {
    int tokenType;
    struct hashNode* symbol;
    struct astNode* child[MAX_CHILDREN];
};

struct astNode* astCreate(int tokenType, struct hashNode* symbol, struct astNode* c0, struct astNode* c1, struct astNode* c2, struct astNode* c3);
void astPrint(struct astNode* node, int level);
void astDecompile(struct astNode* node, FILE* outFile);
const char* getTypeName(struct astNode* node);
