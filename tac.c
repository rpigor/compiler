#include "tac.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

const char* tacTypeNames[] = { "TAC_SYMBOL", "TAC_ADD", "TAC_SUB", "TAC_DIV", "TAC_MUL", "TAC_LT", "TAC_GT", "TAC_DIF", "TAC_EQ", "TAC_GE", "TAC_LE", "TAC_AND", "TAC_OR", "TAC_NOT", "TAC_ASSIGN", "TAC_VEC_ASSIGN", "TAC_VEC_ELEM", "TAC_VECINIT", "TAC_DEC_VEC", "TAC_DEC_VAR", "TAC_JMP_FALSE", "TAC_JMP", "TAC_LABEL", "TAC_RETORNE", "TAC_FUNC_BEGIN", "TAC_FUNC_END", "TAC_FUNC_CALL", "TAC_ARG", "TAC_PARAM", "TAC_ENTRADA", "TAC_ESCREVA" };

const char* functionCallRegisters[] = { "edi", "esi", "edx", "ecx", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };

int actualDefineString = 0;
int argIndex = 0;
int paramIndex = 0;

struct tacNode* tacCreate(int type, struct hashNode* result, struct hashNode* firstOp, struct hashNode* secondOp) {
    struct tacNode* newNode;
    newNode = (struct tacNode*) calloc(1, sizeof(struct tacNode));

    newNode->type = type;
    newNode->result = result;
    newNode->firstOp = firstOp;
    newNode->secondOp = secondOp;
    newNode->previous = NULL;
    newNode->next = NULL;

    return newNode;
}

void tacPrint(struct tacNode* node) {
    if (!node) {
        return;
    }

    if (node->type == TAC_SYMBOL) {
        return;
    }

    fprintf(stderr, "tac(%s", getTacTypeName(node));
    if (node->result) {
        fprintf(stderr, ", %s", node->result->text);
    }
    if (node->firstOp) {
        fprintf(stderr, ", %s", node->firstOp->text);
    }
    if (node->secondOp) {
        fprintf(stderr, ", %s", node->secondOp->text);
    }
    fprintf(stderr, ");\n");
}

void tacPrintBackwards(struct tacNode* node) {
    if (!node) {
        return;
    }

    tacPrintBackwards(node->previous);
    tacPrint(node);
}

struct tacNode* tacJoin(struct tacNode* firstNode, struct tacNode* secondNode) {
    if (!firstNode) {
        return secondNode;
    }

    if (!secondNode) {
        return firstNode;
    }

    struct tacNode* point;
    for (point = secondNode; point->previous; point = point->previous) {
        ;
    }
    point->previous = firstNode;

    return secondNode;
}

const char* getTacTypeName(struct tacNode* node) {
    return tacTypeNames[node->type];
}

int getTacType(struct astNode* node) {
    if (!node) {
        return -1;
    }

    switch (node->tokenType) {
        case AST_SYMBOL: return TAC_SYMBOL;
        case AST_ADD: return TAC_ADD;
        case AST_SUB: return TAC_SUB;
        case AST_DIV: return TAC_DIV;
        case AST_MUL: return TAC_MUL;
        case AST_LT: return TAC_LT;
        case AST_GT: return TAC_GT;
        case AST_DIF: return TAC_DIF;
        case AST_EQ: return TAC_EQ;
        case AST_GE: return TAC_GE;
        case AST_LE: return TAC_LE;
        case AST_AND: return TAC_AND;
        case AST_OR: return TAC_OR;
        case AST_NOT: return TAC_NOT;
        case AST_ASSIGN: return TAC_ASSIGN;
        case AST_FUNC_CALL: return TAC_FUNC_CALL;
        case AST_PRINTL: return TAC_ESCREVA;
        case AST_ARGL: return TAC_ARG;
        case AST_VECINITL: return TAC_VECINIT;
        default: return -1;
    }

    return -1;
}

struct tacNode* makeIfThen(struct tacNode* expression, struct tacNode* thenBody) {
    struct tacNode* tacJump = NULL;
    struct tacNode* tacLabel = NULL;
    struct hashNode* newLabel = makeLabelHash();

    tacJump = tacCreate(TAC_JMP_FALSE, newLabel, expression ? expression->result : NULL, NULL);
    tacJump->previous = expression;

    tacLabel = tacCreate(TAC_LABEL, newLabel, NULL, NULL);
    tacLabel->previous = thenBody;

    return tacJoin(tacJump, tacLabel);
}

struct tacNode* makeIfThenElse(struct tacNode* expression, struct tacNode* thenBody, struct tacNode* elseBody) {
    struct tacNode* tacJumpElse = NULL;
    struct tacNode* tacLabelElse = NULL;
    struct hashNode* elseLabel = makeLabelHash();
    struct tacNode* tacJumpEnd = NULL;
    struct tacNode* tacLabelEnd = NULL;
    struct hashNode* endLabel = makeLabelHash();

    tacJumpElse = tacCreate(TAC_JMP_FALSE, elseLabel, expression ? expression->result : NULL, NULL);
    tacJumpElse->previous = expression;

    tacJumpEnd = tacCreate(TAC_JMP, endLabel, NULL, NULL);
    tacJumpEnd->previous = thenBody;

    tacLabelElse = tacCreate(TAC_LABEL, elseLabel, NULL, NULL);
    tacLabelElse->previous = tacJumpEnd;

    tacLabelEnd = tacCreate(TAC_LABEL, endLabel, NULL, NULL);
    tacLabelEnd->previous = elseBody;

    return tacJoin(tacJumpElse, tacJoin(tacLabelElse, tacLabelEnd));
}

struct tacNode* makeWhile(struct tacNode* expression, struct tacNode* body) {
    struct tacNode* tacConditionLabel = NULL;
    struct hashNode* conditionLabel = makeLabelHash();
    struct tacNode* tacJumpEnd = NULL;
    struct tacNode* tacJumpCondition = NULL;
    struct tacNode* tacEndLabel = NULL;
    struct hashNode* endLabel = makeLabelHash();

    tacConditionLabel = tacCreate(TAC_LABEL, conditionLabel, NULL, NULL);

    tacJumpEnd = tacCreate(TAC_JMP_FALSE, endLabel, expression ? expression->result : NULL, NULL);
    tacJumpEnd->previous = expression;

    tacJumpCondition = tacCreate(TAC_JMP, conditionLabel, NULL, NULL);
    tacJumpCondition->previous = body;

    tacEndLabel = tacCreate(TAC_LABEL, endLabel, NULL, NULL);
    tacEndLabel->previous = tacJumpCondition;

    return tacJoin(tacConditionLabel, tacJoin(tacJumpEnd, tacEndLabel));
}

struct tacNode* makeFunction(struct hashNode* symbol, struct tacNode* parameters, struct tacNode* body) {
    return tacJoin(tacJoin(tacCreate(TAC_FUNC_BEGIN, symbol, NULL, NULL), parameters), tacJoin(body, tacCreate(TAC_FUNC_END, symbol, NULL, NULL)));
}

struct tacNode* generateCode(struct astNode* node) {
    if (!node) {
        return NULL;
    }

    struct tacNode* result = NULL;
    struct tacNode* code[MAX_CHILDREN];

    for (int i = 0; i < MAX_CHILDREN; i++) {
        code[i] = generateCode(node->child[i]);
    }

    switch (node->tokenType) {
        case AST_SYMBOL:
        case AST_LITSYMBOL:
            result = tacCreate(TAC_SYMBOL, node->symbol, NULL, NULL);
            break;
        case AST_ADD:
        case AST_SUB:
        case AST_DIV:
        case AST_MUL:
        case AST_LT:
        case AST_GT:
        case AST_DIF:
        case AST_EQ:
        case AST_GE:
        case AST_LE:
        case AST_AND:
        case AST_OR:
            result = tacJoin(code[0], tacJoin(code[1], tacCreate(getTacType(node), makeTempHash(), code[0] ? code[0]->result : NULL, code[1] ? code[1]->result : NULL)));
            break;
        case AST_NOT:
            result = tacJoin(code[0], tacCreate(getTacType(node), makeTempHash(), code[0] ? code[0]->result : NULL, NULL));
            break;
        case AST_ASSIGN:
        case AST_FUNC_CALL:
            result = tacJoin(code[0], tacCreate(getTacType(node), node->symbol, code[0] ? code[0]->result : NULL, NULL));
            break;
        case AST_VEC_ASSIGN:
            result = tacJoin(code[0], tacJoin(code[1], tacCreate(TAC_VEC_ASSIGN, node->symbol, code[0] ? code[0]->result : NULL, code[1] ? code[1]->result : NULL)));
            break;
        case AST_DEC_VAR:
            result = tacJoin(code[1], tacCreate(TAC_DEC_VAR, node->symbol, code[1] ? code[1]->result : NULL, NULL));
            break;
        case AST_DEC_VEC:
            result = tacJoin(tacJoin(tacCreate(TAC_DEC_VEC, node->symbol, code[1] ? code[1]->result : NULL, NULL), code[2]), code[1]);
            break;
        case AST_DEC_FUN:
            result = makeFunction(node->symbol, code[1], code[2]);
            break;
        case AST_ARGL:
        case AST_PRINTL:
        case AST_VECINITL:
            result = tacJoin(tacJoin(code[0], tacCreate(getTacType(node), NULL, code[0] ? code[0]->result : NULL, code[1] ? code[1]->result : NULL)), code[1]);
            break;
        case AST_PARAM:
            result = tacCreate(TAC_PARAM, node->symbol, NULL, NULL);
            break;
        case AST_VEC_ELEM:
            result = tacJoin(code[0], tacCreate(TAC_VEC_ELEM, makeTempHash(), node->symbol, code[0] ? code[0]->result : NULL));
            break;
        case AST_ENTRADA:
            result = tacCreate(TAC_ENTRADA, makeTempHash(), NULL, NULL);
            break;
        case AST_SE:
            result = makeIfThen(code[1], code[0]);
            break;
        case AST_SE_SENAUM:
            result = makeIfThenElse(code[2], code[0], code[1]);
            break;
        case AST_ENQUANTO:
            result = makeWhile(code[1], code[0]);
            break;
        case AST_RETORNE:
            result = tacJoin(code[0], tacCreate(TAC_RETORNE, code[0] ? code[0]->result : NULL, NULL, NULL));
            break;
        default:
            result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
            break;
    }

    return result;
}

struct tacNode* tacReverse(struct tacNode* node) {
    struct tacNode* tac;
    for (tac = node; tac->previous; tac = tac->previous) {
        tac->previous->next = tac;
    }

    return tac;
}

char* getInstruction(struct tacNode* tac) {
    if (!tac) {
        return NULL;
    }

    switch (tac->type) {
        case TAC_ADD:
            return "addl";
        case TAC_SUB:
            return "subl";
        case TAC_MUL:
            return "imull";
        case TAC_EQ:
            return "sete";
        case TAC_DIF:
            return "setne";
        case TAC_GE:
            return "setge";
        case TAC_GT:
            return "setg";
        case TAC_LE:
            return "setle";
        case TAC_LT:
            return "setl";
        case TAC_AND:
            return "andl";
        case TAC_OR:
            return "orl";
        default:
            return NULL;
    }

    return NULL;
}

void generateAsm(struct tacNode* node, FILE* out) {
    fprintf(out,    "\t.section\t.rodata\n"
                    ".intstr:\n"
                    "\t.string\t\"%%d\"\n"
                    ".stringstr:\n"
                    "\t.string\t\"%%s\"\n"
                    "\t.text\n"
    );

    for (struct tacNode* tac = node; tac; tac = tac->next) {
        switch (tac->type) {
            case TAC_ASSIGN:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->firstOp->text, tac->result->text);
                break;
            case TAC_VEC_ASSIGN:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tcltq\n"
                                "\tmovl\t_%s(,%%rax,4), %%eax\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->firstOp->text, tac->secondOp->text, tac->result->text);
                break;
            case TAC_VEC_ELEM:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tcltq\n"
                                "\tmovl\t_%s(,%%rax,4), %%eax\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->secondOp->text, tac->firstOp->text, tac->result->text);
                break;
            case TAC_ADD:
            case TAC_SUB:
            case TAC_MUL:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tmovl\t_%s(%%rip), %%edx\n"
                                "\t%s\t%%eax, %%edx\n"
                                "\tmovl\t%%edx, _%s(%%rip)\n",
                        tac->firstOp->text, tac->secondOp->text, getInstruction(tac), tac->result->text);
                break;
            case TAC_DIV:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tcltd\n"
                                "\tidivl\t_%s(%%rip)\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->firstOp->text, tac->secondOp->text, tac->result->text);
                break;
            case TAC_EQ:
            case TAC_DIF:
            case TAC_GE:
            case TAC_GT:
            case TAC_LE:
            case TAC_LT:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%edx\n"
                                "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tcmpl\t%%eax, %%edx\n"
                                "\t%s\t%%al\n"
                                "\tmovzbl\t%%al, %%eax\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->firstOp->text, tac->secondOp->text, getInstruction(tac), tac->result->text);
                break;
            case TAC_AND:
            case TAC_OR:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%edx\n"
                                "\tmovl\t_%s(%%rip), %%eax\n"
                                "\t%s\t%%edx, %%eax\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->firstOp->text, tac->secondOp->text, getInstruction(tac), tac->result->text);
                break;
            case TAC_NOT:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tnotl\t%%eax\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->firstOp->text, tac->result->text);
            case TAC_FUNC_BEGIN:
                fprintf(out,    "\t.globl\t%s\n"
                                "\t.type\t%s, @function\n"
                                "%s:\n"
                                "\tpushq\t%%rbp\n"
                                "\tmovq\t%%rsp, %%rbp\n",
                        tac->result->text, tac->result->text, tac->result->text);
                break;
            case TAC_FUNC_END:
                fprintf(out,    "\tpopq\t%%rbp\n"
                                "\tret\n"
                        );
                paramIndex = 0;
                break;
            case TAC_PARAM:
                fprintf(out,    "\tmovl\t%%%s, _%s(%%rip)\n",
                        functionCallRegisters[paramIndex++], tac->result->text);
                break;
            case TAC_ARG:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%%s\n",
                        tac->firstOp->text, functionCallRegisters[argIndex++]);
                break;
            case TAC_FUNC_CALL:
                fprintf(out,    "\tcall\t%s\n"
                                "\tmovl\t%%eax, _%s(%%rip)\n",
                        tac->result->text, tac->result->text);
                argIndex = 0;
                break;
            case TAC_RETORNE:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\tpopq\t%%rbp\n"
                                "\tret\n",
                        tac->result->text);
                break;
            case TAC_ESCREVA:
                if (tac->firstOp->text[0] == '"') {
                    fprintf(out,    "\tmovl\t$.defstr%d, %%esi\n"
                                    "\tmovl\t$.stringstr, %%edi\n"
                                    "\tmovl\t$0, %%eax\n"
                                    "\tcall\tprintf\n",
                            actualDefineString++);
                }
                else {
                    fprintf(out,    "\tmovl\t_%s(%%rip), %%esi\n"
                                    "\tmovl\t$.intstr, %%edi\n"
                                    "\tmovl\t$0, %%eax\n"
                                    "\tcall\tprintf\n",
                            tac->firstOp->text);
                }
                break;
            case TAC_ENTRADA:
                fprintf(out,    "\tmovl\t$_%s, %%esi\n"
                                "\tmovl\t$.intstr, %%edi\n"
                                "\tmovl\t$0, %%eax\n"
                                "\tcall\t__isoc99_scanf\n",
                        tac->result->text);
                break;
            case TAC_JMP:
                fprintf(out,    "\tjmp\t.%s\n", tac->result->text);
                break;
            case TAC_JMP_FALSE:
                fprintf(out,    "\tmovl\t_%s(%%rip), %%eax\n"
                                "\ttestl\t%%eax, %%eax\n"
                                "\tje\t.%s\n",
                        tac->firstOp->text, tac->result->text);
                break;
            case TAC_LABEL:
                fprintf(out,    ".%s:\n", tac->result->text);
                break;
            default:
                break;
        }
    }
}
