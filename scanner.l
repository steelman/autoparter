/* -*- mode: c -*- */
%{
#include "autoparter.h"
#include "parser.h"
#if 0
#define _DEBUG(f...) fprintf(stderr, f)
#else
#define _DEBUG(f...)
#endif
%}

%option noyywrap
%option outfile="autoparter.yy.c"

%%

[ \t]+          /* ignore whitespace characters */
^device     |
^label      |
^partition  |
^filesystem |
^mount          { yylval.word = malloc(sizeof(struct word)); yylval.word->w = strdup(yytext); return KEYWORD; }
[A-Za-z_/]+     { yylval.word = malloc(sizeof(struct word)); yylval.word->w = strdup(yytext); return WORD;    }
=|:             { return yytext[0]; }
.|\n            ;

%%
