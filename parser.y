%{
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

int getLineNumber();
%}

%union
{
	struct hashNode* symbol;
}

%token KW_CARA
%token KW_INTE
%token KW_REAL

%token KW_SE
%token KW_ENTAUM
%token KW_SENAUM
%token KW_ENQUANTO
%token KW_ENTRADA
%token KW_ESCREVA
%token KW_RETORNE

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEIRO
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR

%left '&' '|'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'
%left '~'

%%

program:
    | decl
    ;

decl: dec tail
    ;

tail: dec tail
    |
    ;

dec: type TK_IDENTIFIER '=' lit ';'
    | type TK_IDENTIFIER '[' LIT_INTEIRO ']' vecinitl ';'
    | type TK_IDENTIFIER '(' paraml ')' block
    ;

type: KW_CARA
    | KW_INTE
    | KW_REAL
    ;

lit: LIT_CHAR
    | LIT_FLOAT
    | LIT_INTEIRO
    | LIT_STRING
    ;

vecinitl: lit vecinitl
    |
    ;

paraml: param paraml
    |
    ;

param: type TK_IDENTIFIER
    ;

block: '{' cmdl '}'
    ;

cmdl: cmd ';' cmdl
    | cmd
    ;

cmd: TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    | KW_ESCREVA exprl
    | KW_ENTAUM cmd KW_SE '(' expr ')'
    | KW_ENTAUM cmd KW_SENAUM cmd KW_SE '(' expr ')'
    | cmd KW_ENQUANTO '(' expr ')'
    | KW_RETORNE expr
    | block
    |
    ;

expr: lit
    | TK_IDENTIFIER
    | TK_IDENTIFIER '[' expr ']'
    | expr '+' expr
    | expr '-' expr
    | expr '/' expr
    | expr '*' expr
    | expr '<' expr
    | expr '>' expr
    | expr OPERATOR_DIF expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_LE expr
    | expr '&' expr
    | expr '|' expr
    | '~' expr
    | '(' expr ')'
    | TK_IDENTIFIER '(' argl ')'
    | KW_ENTRADA
    ;

exprl: expr exprl
    |
    ;

argl: arg argl
    |
    ;

arg: lit
    | TK_IDENTIFIER
    ;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
    exit(3);
}
