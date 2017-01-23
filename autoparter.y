%{
#include <stdio.h>
%}

%token KEYWORD WORD
%defines
%define lr.type ielr
%define api.value.type {const char*}

%printer { fprintf(yyoutput, "%s", $$); } WORD KEYWORD

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


parameter:      WORD '=' WORD { fprintf(stderr, "P:%s\n", );}

%%

extern FILE* yyin;

main()
{
#if YYDEBUG
  yydebug=1;
#endif
	do {
		yyparse();
		printf("-----\n");
	} while (!feof(yyin));
}

yyerror (char const *s)
{
	fprintf (stderr, "%s\n", s);
}
