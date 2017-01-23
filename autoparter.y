%{
#include <stdio.h>
%}

%token KEYWORD WORD
%defines
%define lr.type ielr

%%

rules: rules rule | rule
rule:KEYWORD WORD ':' words
words: words WORD | WORD

%%
extern FILE* yyin;

main()
{
	do {
		yyparse();
		printf("-----\n");
	} while (!feof(yyin));
}

yyerror (char const *s)
{
	fprintf (stderr, "%s\n", s);
}
