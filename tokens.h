/*
 * Compiladores - etapa1 - tokens.h - semestre 2022/2 - M Johann
 *
 * Lista dos tokens, com valores constantes associados.
 * Este arquivo serah posterioremente substituido, nao acrescente nada.
 * Os valores das constantes sao arbitrarios, mas nao podem ser alterados.
 * Cada valor deve ser distinto e fora da escala ascii.
 * Assim, nao conflitam entre si e com os tokens representados pelo proprio
 * valor ascii de caracteres isolados.
 */

#define KW_CARA           256
#define KW_INTE           257
#define KW_REAL           258

#define KW_SE             261
#define KW_ENTAUM         262
#define KW_SENAUM         263
#define KW_ENQUANTO       264
#define KW_ENTRADA        266
#define KW_ESCREVA        267
#define KW_RETORNE        268

#define OPERATOR_LE       270
#define OPERATOR_GE       271
#define OPERATOR_EQ       272
#define OPERATOR_DIF      273

#define TK_IDENTIFIER     280

#define LIT_INTEIRO       281
#define LIT_FLOAT         282
#define LIT_CHAR          285
#define LIT_STRING        286

#define TOKEN_ERROR       290

/* END OF FILE */

