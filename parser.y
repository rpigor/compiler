%{
#include "hash.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

int getLineNumber();

struct astNode* root = NULL;
%}

%union
{
	struct hashNode* symbol;
	struct astNode* ast;
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

%type<ast> program
%type<ast> decl
%type<ast> tail
%type<ast> dec
%type<ast> litint
%type<ast> type
%type<ast> lit
%type<ast> vecinitl
%type<ast> paraml
%type<ast> param
%type<ast> block
%type<ast> cmdl
%type<ast> cmd
%type<ast> expr
%type<ast> exprl
%type<ast> argl
%type<ast> arg

%left '&' '|'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'
%left '~'

%%

program:                                                    { $$ = 0; }
    | decl                                                  { $$ = $1; root = $$; astPrint(root, 0); }
    ;

decl: dec tail                                              { $$ = astCreate(AST_DECL, 0, $1, $2, 0, 0); }
    ;

tail: dec tail                                              { $$ = astCreate(AST_TAIL, 0, $1, $2, 0, 0); }
    |                                                       { $$ = 0; }
    ;

dec: type TK_IDENTIFIER '=' lit ';'                         { $$ = astCreate(AST_DEC_VAR, $2, $1, $4, 0, 0); }
    | type TK_IDENTIFIER '[' litint ']' vecinitl ';'        { $$ = astCreate(AST_DEC_VEC, $2, $1, $4, $6, 0); }
    | type TK_IDENTIFIER '(' paraml ')' block               { $$ = astCreate(AST_DEC_FUN, $2, $1, $4, $6, 0); }
    ;

litint: LIT_INTEIRO                                         { $$ = astCreate(AST_LITSYMBOL, $1, 0, 0, 0, 0); }
    ;

type: KW_CARA                                               { $$ = astCreate(AST_CARA, 0, 0, 0, 0, 0); }
    | KW_INTE                                               { $$ = astCreate(AST_INTE, 0, 0, 0, 0, 0); }
    | KW_REAL                                               { $$ = astCreate(AST_REAL, 0, 0, 0, 0, 0); }
    ;

lit: LIT_CHAR                                               { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_FLOAT                                             { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_INTEIRO                                           { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_STRING                                            { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

vecinitl: lit vecinitl                                      { $$ = astCreate(AST_VECINITL, 0, $1, $2, 0, 0); }
    |                                                       { $$ = 0; }
    ;

paraml: param paraml                                        { $$ = astCreate(AST_PARAML, 0, $1, $2, 0, 0); }
    |                                                       { $$ = 0; }
    ;

param: type TK_IDENTIFIER                                   { $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0); }
    ;

block: '{' cmdl '}'                                         { $$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0); }
    ;

cmdl: cmd ';' cmdl                                          { $$ = astCreate(AST_CMDL, 0, $1, $3, 0, 0); }
    | cmd                                                   { $$ = astCreate(AST_CMDL, 0, $1, 0, 0, 0); }
    ;

cmd: TK_IDENTIFIER '=' expr                                 { $$ = astCreate(AST_ASSIGN, $1, $3, 0, 0, 0); }
    | TK_IDENTIFIER '[' expr ']' '=' expr                   { $$ = astCreate(AST_VEC_ASSIGN, $1, $3, $6, 0, 0); }
    | KW_ESCREVA exprl                                      { $$ = astCreate(AST_ESCREVA, 0, $2, 0, 0, 0); }
    | KW_ENTAUM cmd KW_SE '(' expr ')'                      { $$ = astCreate(AST_SE, 0, $2, $5, 0, 0); }
    | KW_ENTAUM cmd KW_SENAUM cmd KW_SE '(' expr ')'        { $$ = astCreate(AST_SE_SENAUM, 0, $2, $4, $7, 0); }
    | cmd KW_ENQUANTO '(' expr ')'                          { $$ = astCreate(AST_ENQUANTO, 0, $1, $4, 0, 0); }
    | KW_RETORNE expr                                       { $$ = astCreate(AST_RETORNE, 0, $2, 0, 0, 0); }
    | block                                                 { $$ = $1; }
    |                                                       { $$ = 0; }
    ;

expr: lit                                                   { $$ = $1; }
    | TK_IDENTIFIER                                         { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | TK_IDENTIFIER '[' expr ']'                            { $$ = astCreate(AST_VEC_ELEM, $1, $3, 0, 0, 0); }
    | expr '+' expr                                         { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }
    | expr '-' expr                                         { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }
    | expr '/' expr                                         { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
    | expr '*' expr                                         { $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0); }
    | expr '<' expr                                         { $$ = astCreate(AST_LT, 0, $1, $3, 0, 0); }
    | expr '>' expr                                         { $$ = astCreate(AST_GT, 0, $1, $3, 0, 0); }
    | expr OPERATOR_DIF expr                                { $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0); }
    | expr OPERATOR_EQ expr                                 { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
    | expr OPERATOR_GE expr                                 { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
    | expr OPERATOR_LE expr                                 { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
    | expr '&' expr                                         { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
    | expr '|' expr                                         { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
    | '~' expr                                              { $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0); }
    | '(' expr ')'                                          { $$ = astCreate(AST_BRAC_EXPR, 0, $2, 0, 0, 0); }
    | TK_IDENTIFIER '(' argl ')'                            { $$ = astCreate(AST_FUNC_CALL, $1, $3, 0, 0, 0); }
    | KW_ENTRADA                                            { $$ = astCreate(AST_ENTRADA, 0, 0, 0, 0, 0); }
    ;

exprl: expr exprl                                           { $$ = astCreate(AST_EXPRL, 0, $1, $2, 0, 0); }
    |                                                       { $$ = 0; }
    ;

argl: arg argl                                              { $$ = astCreate(AST_ARGL, 0, $1, $2, 0, 0); }
    |                                                       { $$ = 0; }
    ;

arg: lit                                                    { $$ = $1; }
    | TK_IDENTIFIER                                         { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
    exit(3);
}
