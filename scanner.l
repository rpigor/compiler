%{
#include "y.tab.h"
#include "hash.h"

int lineNumber = 1;
int running = 1;
%}

%x LINE_COMMENT
%x BLOCK_COMMENT

%%

"cara"                      { return KW_CARA; }
"inte"                      { return KW_INTE; }
"real"                      { return KW_REAL; }

"se"                        { return KW_SE; }
"entaum"                    { return KW_ENTAUM; }
"senaum"                    { return KW_SENAUM; }
"enquanto"                  { return KW_ENQUANTO; }
"entrada"                   { return KW_ENTRADA; }
"escreva"                   { return KW_ESCREVA; }
"retorne"                   { return KW_RETORNE; }

"<="                        { return OPERATOR_LE; }
">="                        { return OPERATOR_GE; }
"=="                        { return OPERATOR_EQ; }
"!="                        { return OPERATOR_DIF; }

\"(\\\"|[^"])*\"            { yylval.symbol = insertHash(SYMBOL_LIT_STRING, yytext); return LIT_STRING; }
'(.|\\[a-z0-9])'            { yylval.symbol = insertHash(SYMBOL_LIT_CARA, yytext); return LIT_CHAR; }
(\+|-)?[0-9]*\.[0-9]+       { yylval.symbol = insertHash(SYMBOL_LIT_REAL, yytext); return LIT_FLOAT; }
(\+|-)?[0-9]+               { yylval.symbol = insertHash(SYMBOL_LIT_INTE, yytext); return LIT_INTEIRO; }

[a-z._][a-z0-9._]*          { yylval.symbol = insertHash(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }

[;()\[\]{}=+\-*/<>&|~]      { return yytext[0]; }
[ \t,]
"\n"                        { ++lineNumber; }
"//"                        { BEGIN(LINE_COMMENT); }
"///"                       { BEGIN(BLOCK_COMMENT); }
.                           { return TOKEN_ERROR; }

<LINE_COMMENT>"\n"          { ++lineNumber; BEGIN(INITIAL); }
<LINE_COMMENT>.

<BLOCK_COMMENT>"\n"         { ++lineNumber; }
<BLOCK_COMMENT>.
<BLOCK_COMMENT>"\\\\\\"     { BEGIN(INITIAL); }

%%

#include "main.c"

void initMe(void) {
    initHash();
}

int isRunning(void) {
    return running;
}

int getLineNumber(void) {
    return lineNumber;
}

int yywrap() {
    running = 0;
    return 1;
}
