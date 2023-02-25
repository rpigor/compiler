#include <stdio.h>

void initMe(void);
int yyparse();

int main(int argc, char** argv) {
    initMe();

    if (argc < 2) {
        printf("Call: ./etapa2 input.txt\n");
        exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == 0) {
        printf("Cannot open file %s...\n", argv[1]);
        exit(2);
    }

    yyparse();

    /*int tok;
    while (running) {
        tok = yylex();
        if (!running) {
            break;
        }

        switch (tok) {
            case KW_CARA:
            case KW_INTE:
            case KW_REAL:
            case KW_SE:
            case KW_SENAUM:
            case KW_ENQUANTO:
            case KW_ENTRADA:
            case KW_RETORNE:
            case OPERATOR_LE:
            case OPERATOR_GE:
            case OPERATOR_EQ:
            case OPERATOR_DIF:
            case LIT_STRING:
            case LIT_CHAR:
            case LIT_FLOAT:
            case LIT_INTEIRO:
            case TK_IDENTIFIER:
            default:
                printf("%d: %s\n", tok, yytext);
                break;
        }
    }*/

    printf("File has %d lines.\n", lineNumber);
    printf("Hash table:\n");
    printHash();

    return 0;
}
