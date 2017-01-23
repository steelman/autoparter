%{
#include "autoparter.tab.h"
%}

%option noyywrap
%option outfile="autoparter.yy.c"

%%

[ \t]+
^foo |
^bar            { printf("KEYWORD: %s\n", yytext); return KEYWORD; }
[A-Za-z]+       { printf("WORD: %s\n", yytext); return WORD; }
:               { printf("COLON\n"); return ':'; }
.|\n ;

%%

_main()
{
	yylex();
}
