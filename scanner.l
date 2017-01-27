/* -*- mode: c -*- */
%{
#define _GNU_SOURCE
#include "autoparter.h"
#include "parser.h"
#if 0
#define _DEBUG(f...) fprintf(stderr, f)
#else
#define _DEBUG(f...)
#endif
%}

%option noyywrap
%option outfile="scanner.c"

%%

[ \t]+          /* ignore whitespace characters */

^device     |
^label      |
^partition  |
^filesystem |
^mount          {
     yylval.word = malloc(sizeof(struct word));
     yylval.word->w = strdup(yytext);
     return KEYWORD;
}

[-_/A-Za-z0-9]* {
    yylval.word = malloc(sizeof(struct word));
    yylval.word->w = strdup(yytext);
    return WORD;
}

\"[^\"\n]*[\"\n] {
    if (yytext[yyleng-1] != '"')
        return ERROR;
    yytext[yyleng-1] = '\0';
    yylval.word = malloc(sizeof(struct word));
    yylval.word->w = strdup(yytext+1);
    return WORD;
}

=|:             { return yytext[0]; }
\n              { autoparter_lineno++; }
.               ;

%%
