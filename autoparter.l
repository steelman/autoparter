%{
#include "autoparter.tab.h"
#if 0
#define _DEBUG(f...) printf(f)
#else
#define _DEBUG(f...)
#endif
%}

%option bison-bridge
%option noyywrap
%option outfile="autoparter.yy.c"

%%

[ \t]+          /* ignore whitespace characters */
^device     |
^label      |
^partition  |
^filesystem |
^mount          { _DEBUG("KEYWORD: %s\n", yytext); return KEYWORD; }
[A-Za-z_/]+     { _DEBUG("WORD: %s\n", yytext); yylval = yytext; return WORD;    }
=|:             { _DEBUG("%c", yytext[0]); return yytext[0]; }
.|\n            ;

%%
