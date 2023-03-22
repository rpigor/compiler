#pragma once

#include "hash.h"
#include "ast.h"

enum tacTypes {
    TAC_SYMBOL = 0,
    TAC_ADD,
    TAC_SUB,
    TAC_DIV,
    TAC_MUL,
    TAC_LT,
    TAC_GT,
    TAC_DIF,
    TAC_EQ,
    TAC_GE,
    TAC_LE,
    TAC_AND,
    TAC_OR,
    TAC_NOT,
    TAC_ASSIGN,
    TAC_VEC_ASSIGN,
    TAC_VEC_ELEM,
    TAC_VECINIT,
    TAC_DEC_VEC,
    TAC_DEC_VAR,
    TAC_JMP_FALSE,
    TAC_JMP,
    TAC_LABEL,
    TAC_RETORNE,
    TAC_FUNC_BEGIN,
    TAC_FUNC_END,
    TAC_FUNC_CALL,
    TAC_ARG,
    TAC_PARAM,
    TAC_ENTRADA,
    TAC_ESCREVA
};

struct tacNode {
    int type;
    struct hashNode* result;
    struct hashNode* firstOp;
    struct hashNode* secondOp;
    struct tacNode* previous;
    struct tacNode* next;
};

struct tacNode* tacCreate(int type, struct hashNode* result, struct hashNode* firstOp, struct hashNode* secondOp);
void tacPrint(struct tacNode* node);
void tacPrintBackwards(struct tacNode* node);
struct tacNode* tacJoin(struct tacNode* firstNode, struct tacNode* secondNode);
const char* getTacTypeName(struct tacNode* node);
int getTacType(struct astNode* node);
struct tacNode* makeIfThen(struct tacNode* firstCode, struct tacNode* secondCode);
struct tacNode* makeIfThenElse(struct tacNode* firstCode, struct tacNode* secondCode, struct tacNode* thirdCode);
struct tacNode* makeWhile(struct tacNode* firstCode, struct tacNode* secondCode);
struct tacNode* makeFunction(struct hashNode* symbol, struct tacNode* parameters, struct tacNode* body);

struct tacNode* generateCode(struct astNode* node);

struct tacNode* tacReverse(struct tacNode* node);
void generateAsm(struct tacNode* node, FILE* out);
