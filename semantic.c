#include "semantic.h"
#include "hash.h"
#include <stdio.h>

int semanticErrors = 0;

void checkAndSetDeclarations(struct astNode* node) {
    if (!node) {
        return;
    }

    /*switch (node->tokenType) {
        case AST_DEC_VAR:
            if (!node->symbol) {
                break;
            }

            if (node->symbol->tokenType != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Semantic error: variable %s is already declared.\n", node->symbol->text);
                semanticErrors++;
            }

            node->symbol->tokenType = SYMBOL_VARIABLE;
            node->symbol->dataType = getDataType(node->symbol);
            break;
        case AST_DEC_FUN:
            if (!node->symbol) {
                break;
            }

            if (node->symbol->tokenType != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Semantic error: fuction %s is already declared.\n", node->symbol->text);
                semanticErrors++;
            }

            node->symbol->tokenType = SYMBOL_FUNCTION;
            break;
        case AST_DEC_VEC:
            if (!node->symbol) {
                break;
            }

            if (node->symbol->tokenType != SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Semantic error: vector %s is already declared.\n", node->symbol->text);
                semanticErrors++;
            }

            node->symbol->tokenType = SYMBOL_VECTOR;
            break;
        default:
            break;
    }*/

    if (node->symbol &&
        (node->tokenType == AST_DEC_VAR || node->tokenType == AST_DEC_FUN || node->tokenType == AST_DEC_VEC || node->tokenType == AST_PARAM)) {
        if (node->symbol->tokenType != SYMBOL_IDENTIFIER) {
            fprintf(stderr, "Semantic error: %s is already declared.\n", node->symbol->text);
            semanticErrors++;
        }

        node->symbol->tokenType = getSymbol(node);
        node->symbol->dataType = getDataType(node);
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        checkAndSetDeclarations(node->child[i]);
    }
}

void checkUndeclared() {
    semanticErrors += checkUndeclaredHash();
}

void checkOperands(struct astNode* node) {
    if (!node) {
        return;
    }

    switch (node->tokenType) {
        case AST_ADD:
        case AST_SUB:
        case AST_DIV:
        case AST_MUL:
        case AST_AND:
        case AST_OR:
            if (!isArithmetic(node->child[0])) {
                fprintf(stderr, "Semantic error: invalid left operand in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            else if (!isArithmetic(node->child[1])) {
                fprintf(stderr, "Semantic error: invalid right operand in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_NOT:
            if (!isArithmetic(node->child[0])) {
                fprintf(stderr, "Semantic error: invalid operand in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_LT:
        case AST_GT:
        case AST_DIF:
        case AST_EQ:
        case AST_GE:
        case AST_LE:
            if (isArithmetic(node->child[0]) && !isArithmetic(node->child[1])) {
                fprintf(stderr, "Semantic error: incompatible operands in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (!isArithmetic(node->child[0]) && isArithmetic(node->child[1])) {
                fprintf(stderr, "Semantic error: incompatible operands in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (isFloatingPointer(node->child[0]) && !isFloatingPointer(node->child[1])) {
                fprintf(stderr, "Semantic error: invalid right operand in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (!isFloatingPointer(node->child[0]) && isFloatingPointer(node->child[1])) {
                fprintf(stderr, "Semantic error: invalid right operand in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        default:
            break;
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        checkOperands(node->child[i]);
    }
}

void checkNature(struct astNode* node) {
    if (!node) {
        return;
    }

    switch (node->tokenType) {
        case AST_ASSIGN:
            if (node->symbol->tokenType != SYMBOL_VARIABLE) {
                fprintf(stderr, "Semantic error: invalid variable in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (!isCompatible(node->child[0], node->symbol->dataType)) {
                fprintf(stderr, "Semantic error: incompatible type in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_VEC_ASSIGN:
            if (node->symbol->tokenType != SYMBOL_VECTOR) {
                fprintf(stderr, "Semantic error: invalid vector in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (!isArithmetic(node->child[0])) {
                fprintf(stderr, "Semantic error: vector index must be an integer in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (!isCompatible(node->child[1], node->symbol->dataType)) {
                fprintf(stderr, "Semantic error: incompatible type in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_VEC_ELEM:
            if (node->symbol->tokenType != SYMBOL_VECTOR) {
                fprintf(stderr, "Semantic error: invalid vector in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            if (!isArithmetic(node->child[0])) {
                fprintf(stderr, "Semantic error: vector index must be an integer in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_FUNC_CALL:
            if (node->symbol->tokenType != SYMBOL_FUNCTION) {
                fprintf(stderr, "Semantic error: invalid function in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_SE:
        case AST_ENQUANTO:
            if (!isRelational(node->child[1])) {
                fprintf(stderr, "Semantic error: nonrelational expression in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        case AST_SE_SENAUM:
            if (!isRelational(node->child[2])) {
                fprintf(stderr, "Semantic error: nonrelational expression in %s.\n", getTypeName(node));
                semanticErrors++;
            }
            break;
        default:
            break;
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        checkNature(node->child[i]);
    }
}

void checkReturns(struct astNode* node) {
    if (!node) {
        return;
    }

    if (node->tokenType == AST_DEC_FUN) {
        struct astNode* functionReturn = findFunctionReturn(node);
        if (!functionReturn) {
            fprintf(stderr, "Semantic error: no return statement found in %s.\n", node->symbol->text);
            semanticErrors++;
        }
        else if (!isCompatible(functionReturn->child[0], node->symbol->dataType)) {
            fprintf(stderr, "Semantic error: incompatible return statement in %s.\n", node->symbol->text);
            semanticErrors++;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++) {
        checkReturns(node->child[i]);
    }
}

int getSemanticErrors() {
    return semanticErrors;
}

void checkSemantic(struct astNode* node) {
    checkAndSetDeclarations(node);
    checkUndeclared();
    checkOperands(node);
    checkNature(node);
    checkReturns(node);
}

int isCompatible(struct astNode* node, int dataType) {
    if (node->symbol && node->symbol->dataType == dataType) {
        return 1;
    }

    if ((dataType == DATATYPE_CARA || dataType == DATATYPE_INTE) && isArithmetic(node)) {
        return 1;
    }

    return 0;
}

int isArithmetic(struct astNode* node) {
    if ((node->tokenType == AST_SYMBOL && (node->symbol->tokenType == SYMBOL_LIT_INTE || node->symbol->tokenType == SYMBOL_LIT_CARA)) ||
        (node->symbol && node->symbol->tokenType == SYMBOL_VARIABLE && (node->symbol->dataType == DATATYPE_CARA || node->symbol->dataType == DATATYPE_INTE)) ||
        (node->tokenType == AST_FUNC_CALL && (node->symbol->dataType == DATATYPE_INTE || node->symbol->dataType == DATATYPE_CARA)) ||
        (node->tokenType == AST_BRAC_EXPR && isArithmetic(node->child[0])) ||
        node->tokenType == AST_AND && isArithmetic(node->child[0]) && isArithmetic(node->child[1]) ||
        node->tokenType == AST_OR && isArithmetic(node->child[0]) && isArithmetic(node->child[1]) ||
        node->tokenType == AST_NOT && isArithmetic(node->child[0]) ||
        node->tokenType == AST_ADD && isArithmetic(node->child[0]) && isArithmetic(node->child[1]) ||
        node->tokenType == AST_SUB && isArithmetic(node->child[0]) && isArithmetic(node->child[1]) ||
        node->tokenType == AST_DIV && isArithmetic(node->child[0]) && isArithmetic(node->child[1]) ||
        node->tokenType == AST_MUL && isArithmetic(node->child[0]) && isArithmetic(node->child[1])) {
        return 1;
    }

    return 0;
}

int isRelational(struct astNode* node) {
    if ((node->tokenType == AST_BRAC_EXPR && isRelational(node->child[0])) ||
        node->tokenType == AST_LT ||
        node->tokenType == AST_GT ||
        node->tokenType == AST_DIF ||
        node->tokenType == AST_EQ ||
        node->tokenType == AST_GE ||
        node->tokenType == AST_LE) {
        return 1;
    }

    return 0;
}

int isFloatingPointer(struct astNode* node) {
    if ((node->tokenType == AST_SYMBOL && node->symbol->tokenType == SYMBOL_LIT_REAL) ||
        (node->symbol && node->symbol->tokenType == SYMBOL_VARIABLE && node->symbol->dataType == DATATYPE_REAL) ||
        (node->tokenType == AST_FUNC_CALL && node->symbol->dataType == DATATYPE_REAL) ||
        (node->tokenType == AST_BRAC_EXPR && isFloatingPointer(node->child[0])) ||
        node->tokenType == AST_ADD && isFloatingPointer(node->child[0]) && isFloatingPointer(node->child[1]) ||
        node->tokenType == AST_SUB && isFloatingPointer(node->child[0]) && isFloatingPointer(node->child[1]) ||
        node->tokenType == AST_DIV && isFloatingPointer(node->child[0]) && isFloatingPointer(node->child[1]) ||
        node->tokenType == AST_MUL && isFloatingPointer(node->child[0]) && isFloatingPointer(node->child[1])) {
        return 1;
    }

    return 0;
}

struct astNode* findFunctionReturn(struct astNode* node) {
    if (!node) {
        return NULL;
    }

	if (node->tokenType == AST_RETORNE) {
        return node;
	}

	for (int i = 0; i < MAX_CHILDREN; i++) {
        struct astNode* returnNode = findFunctionReturn(node->child[i]);
		if (!returnNode) {
            continue;
        }
        return returnNode;
	}

    return NULL;
}

int getSymbol(struct astNode* node) {
    switch (node->tokenType) {
        case AST_DEC_FUN:
            return SYMBOL_FUNCTION;
        case AST_DEC_VEC:
            return SYMBOL_VECTOR;
        case AST_DEC_VAR:
        case AST_PARAM:
            return SYMBOL_VARIABLE;
        default:
            return -1;
    }

    return -1;
}

int getDataType(struct astNode* node) {
    switch (node->child[0]->tokenType) {
        case AST_CARA:
            return DATATYPE_CARA;
        case AST_INTE:
            return DATATYPE_INTE;
        case AST_REAL:
            return DATATYPE_REAL;
        default:
            return -1;
    }

    return -1;
}