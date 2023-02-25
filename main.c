#include <stdio.h>
#include <stdlib.h>

void initMe();
int yyparse();
void checkSemantic(struct astNode* node);
int getSemanticErrors();
void astDecompile(struct astNode* node, FILE* outFile);

extern struct astNode* root;

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

    printf("File has %d lines.\n", lineNumber);
    printf("Hash table:\n");
    printHash();

    astDecompile(root, outFile);
    checkSemantic(root);

    fclose(yyin);
    fclose(outFile);

    int errors = getSemanticErrors();
    printf("There were %d semantic errors.\n", errors);
    if (errors > 0) {
        exit(4);
    }

    return 0;
}
