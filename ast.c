#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

const char* typesNames[] = { "AST_DECL", "AST_TAIL", "AST_DEC_VAR", "AST_DEC_FUN", "AST_DEC_VEC", "AST_CARA", "AST_INTE", "AST_REAL", "AST_LITSYMBOL", "AST_SYMBOL", "AST_VECINITL", "AST_PARAML", "AST_PARAM", "AST_BLOCK", "AST_CMDL", "AST_ASSIGN", "AST_VEC_ASSIGN", "AST_ESCREVA", "AST_SE", "AST_SE_SENAUM", "AST_ENQUANTO", "AST_RETORNE", "AST_VEC_ELEM", "AST_ADD", "AST_SUB", "AST_DIV", "AST_MUL", "AST_LT", "AST_GT", "AST_DIF", "AST_EQ", "AST_GE", "AST_LE", "AST_AND", "AST_OR", "AST_NOT", "AST_BRAC_EXPR", "AST_FUNC_CALL", "AST_ENTRADA", "AST_EXPRL", "AST_ARGL" };

struct astNode* astCreate(int tokenType, struct hashNode* symbol, struct astNode* c0, struct astNode* c1, struct astNode* c2, struct astNode* c3) {
    struct astNode* newNode;
    newNode = (struct astNode*) calloc(1, sizeof(struct astNode));

    newNode->tokenType = tokenType;
    newNode->symbol = symbol;
    newNode->child[0] = c0;
    newNode->child[1] = c1;
    newNode->child[2] = c2;
    newNode->child[3] = c3;

    return newNode;
}

void astPrint(struct astNode* node, int level) {
    if (!node) {
        return;
    }

    for (int i = 0; i < level; i++) {
        fprintf(stderr, "  ");
    }

    fprintf(stderr, "ast(");
    fprintf(stderr, "%s\n", getTypeName(node));

    for (int i = 0; i < level; i++) {
        fprintf(stderr, "  ");
    }

    if (node->symbol) {
        fprintf(stderr, " %s", node->symbol->text);
    }

    fprintf(stderr, "\n");

    for (int i = 0; i < MAX_CHILDREN; i++) {
        astPrint(node->child[i], level + 1);
    }

    for (int i = 0; i < level; i++) {
        fprintf(stderr, "  ");
    }

    fprintf(stderr, ")\n");
}

void astDecompile(struct astNode* node, FILE* outFile) {
	if (!node) {
        return;
    }

	switch (node->tokenType) {
        case AST_DECL:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            break;
        case AST_TAIL:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            break;
        case AST_DEC_VAR:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "%s", node->symbol->text);
            fprintf(outFile, " = ");
            astDecompile(node->child[1], outFile);
            fprintf(outFile, ";\n");
            break;
        case AST_DEC_FUN:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "%s", node->symbol->text);
            fprintf(outFile, "( ");
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            fprintf(outFile, ") ");
            astDecompile(node->child[2], outFile);
            fprintf(outFile, "\n");
            break;
        case AST_DEC_VEC:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "%s", node->symbol->text);
            fprintf(outFile, "[ ");
            astDecompile(node->child[1], outFile);
            fprintf(outFile, "] ");
            if (node->child[2]) {
                astDecompile(node->child[2], outFile);
            }
            fprintf(outFile, ";\n");
            break;
        /*case AST_DEC:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "%s", node->symbol->text);
            if (!node->child[1] || (node->child[1] && node->child[1]->tokenType == AST_PARAML)) { // function call
                fprintf(outFile, "( ");
                if (node->child[1]) {
                    astDecompile(node->child[1], outFile);
                }
                fprintf(outFile, ") ");
                astDecompile(node->child[2], outFile);
            }
            else if (node->child[1] && node->child[1]->tokenType == AST_LITSYMBOL) { // vector initialization
                fprintf(outFile, "[ ");
                astDecompile(node->child[1], outFile);
                fprintf(outFile, "] ");
                if (node->child[2]) {
                    astDecompile(node->child[2], outFile);
                }
                fprintf(outFile, ";");
            }
            else { // assignment
                fprintf(outFile, " = ");
                astDecompile(node->child[1], outFile);
                fprintf(outFile, ";");
            }
            fprintf(outFile, "\n");
            break;*/
        case AST_CARA:
            fprintf(outFile, "cara ");
            break;
        case AST_INTE:
            fprintf(outFile, "inte ");
            break;
        case AST_REAL:
            fprintf(outFile, "real ");
            break;
        case AST_LITSYMBOL:
        case AST_SYMBOL:
            fprintf(outFile, "%s ", node->symbol->text);
            break;
        case AST_VECINITL:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            break;
        case AST_PARAML:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            break;
        case AST_PARAM:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "%s ", node->symbol->text);
            break;
        case AST_BLOCK:
            fprintf(outFile, "{\n");
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "}\n");
            break;
        case AST_CMDL:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                fprintf(outFile, ";\n");
                astDecompile(node->child[1], outFile);
            }
            break;
        case AST_ASSIGN:
            fprintf(outFile, "%s = ", node->symbol->text);
            astDecompile(node->child[0], outFile);
            break;
        case AST_VEC_ASSIGN:
            fprintf(outFile, "%s[ ", node->symbol->text);
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "] = ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_ESCREVA:
            fprintf(outFile, "escreva ");
            astDecompile(node->child[0], outFile);
            break;
        case AST_SE:
            fprintf(outFile, "entaum ");
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "se ( ");
            astDecompile(node->child[1], outFile);
            fprintf(outFile, ")");
            break;
        case AST_SE_SENAUM:
            fprintf(outFile, "entaum ");
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "senaum ");
            astDecompile(node->child[1], outFile);
            fprintf(outFile, "se ( ");
            astDecompile(node->child[2], outFile);
            fprintf(outFile, ")");
            break;
        case AST_ENQUANTO:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "enquanto ( ");
            astDecompile(node->child[1], outFile);
            fprintf(outFile, ")");
            break;
        case AST_RETORNE:
            fprintf(outFile, "retorne ");
            astDecompile(node->child[0], outFile);
            break;
        case AST_VEC_ELEM:
            fprintf(outFile, "%s[ ", node->symbol->text);
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "]");
            break;
        case AST_ADD:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "+ ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_SUB:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "- ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_DIV:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "/ ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_MUL:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "* ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_LT:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "< ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_GT:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "> ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_DIF:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "!= ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_EQ:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "== ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_GE:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, ">= ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_LE:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "<= ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_AND:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "& ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_OR:
            astDecompile(node->child[0], outFile);
            fprintf(outFile, "| ");
            astDecompile(node->child[1], outFile);
            break;
        case AST_NOT:
            fprintf(outFile, "~ ");
            astDecompile(node->child[0], outFile);
            break;
        case AST_BRAC_EXPR:
            fprintf(outFile, "( ");
            astDecompile(node->child[0], outFile);
            fprintf(outFile, ")");
            break;
        case AST_FUNC_CALL:
            fprintf(outFile, "%s( ", node->symbol->text);
            astDecompile(node->child[0], outFile);
            fprintf(outFile, ")");
            break;
        case AST_ENTRADA:
            fprintf(outFile, "entrada");
            break;
        case AST_EXPRL:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            break;
        case AST_ARGL:
            astDecompile(node->child[0], outFile);
            if (node->child[1]) {
                astDecompile(node->child[1], outFile);
            }
            break;
        default:
            break;
    }
}

const char* getTypeName(struct astNode* node) {
    return typesNames[node->tokenType];
}