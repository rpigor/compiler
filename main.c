#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "tac.h"
#include "y.tab.h"
#include "lex.yy.h"
#include <stdio.h>
#include <stdlib.h>

int yyparse();
int getLineNumber();
void initMe();

extern struct astNode* root;
extern int lineNumber;

int main(int argc, char** argv) {
    FILE* outFile;

    initMe();

    if (argc < 3) {
        printf("Call: ./etapa3 input.txt output.txt\n");
        exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == NULL) {
        printf("Cannot open file %s...\n", argv[1]);
        exit(2);
    }

    if ((outFile = fopen(argv[2], "w")) == NULL) {
        printf("Cannot open file %s...\n", argv[2]);
        exit(2);
    }

    yyparse();

    printf("File has %d lines.\n", getLineNumber());
    printf("Hash table:\n");
    printHash();

    astDecompile(root, outFile);

    fclose(yyin);
    fclose(outFile);

    tacPrintBackwards(generateCode(root));
    checkSemantic(root);

    int errors = getSemanticErrors();
    printf("There were %d semantic errors.\n", errors);
    if (errors > 0) {
        exit(4);
    }

    return 0;
}
