#pragma once

#include "ast.h"

extern int semanticErrors;

void checkAndSetDeclarations(struct astNode* node);
void checkUndeclared();
void checkOperands(struct astNode* node);
void checkSemantic(struct astNode* node);
void checkNature(struct astNode* node);
void checkReturns(struct astNode* node);
int getSemanticErrors();
int isCompatible(struct astNode* node, int dataType);
int isArithmetic(struct astNode* node);
int isRelational(struct astNode* node);
int isFloatingPointer(struct astNode* node);
struct astNode* findFunctionReturn(struct astNode* node);
int getSymbol(struct astNode* node);
int getDataType(struct astNode* node);
