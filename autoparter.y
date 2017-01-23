%{
#include <stdio.h>
#define YYSTYPE char const *
%}

%token KEYWORD WORD
%defines
%define lr.type ielr

%%

rules:          rule
                | rules rule

rule:           KEYWORD WORD parameters ':' prerequisits

prerequisits:   %empty
                | words

parameters:     parameter
                | parameters parameter

words:          WORD
                | words WORD


parameter:      WORD '=' WORD

%%

extern FILE* yyin;

main()
{
  yydebug=1;
	do {
		yyparse();
		printf("-----\n");
	} while (!feof(yyin));
}

yyerror (char const *s)
{
	fprintf (stderr, "%s\n", s);
}
