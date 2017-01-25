/* -*- mode: c -*- */
%{
#include "autoparter.h"
#include <stdio.h>
#include <stdlib.h>
%}

%defines
%define lr.type ielr
%parse-param {const struct rule **rules_out}

%union {
    struct parameter *param;
    struct word *word;
    struct rule *rule;
}

%token ERROR
%token <word> KEYWORD WORD
%type <param> parameter parameters
%type <word> words prerequisites
%type <rule> rule rules

%printer { fprintf(yyoutput, "%p %s %p", $$, $$->w, $$->next); } WORD KEYWORD
%printer { fprintf(yyoutput, "%p %s => %s %p",
                   $$,
                   $$->name,
                   $$->value,
                   $$->next); } <param>
%printer { fprintf(yyoutput, "%p [type: %s name: %s parameters: %p"
                   " prerequisites: %p] %p",
                   $$,
                   getrulename($$->type),
                   $$->name,
                   $$->parameters,
                   $$->prerequisites,
                   $$->next);} <rule>

%%

start:          rules { *rules_out = $1; }

rules:          rule { $$ = $1; $$->next = NULL; }
                | rules rule { $$ = $2; $$->next = $1; }

rule:           KEYWORD WORD parameters ':' prerequisites {
    $$ = malloc(sizeof(struct rule));

    $$->type = getruletype($1->w);
    free((char*)$1->w);
    free($1);

    $$->name = $2->w;
    free($2);

    $$->parameters = $3;

    $$->prerequisites = $5;
 }

prerequisites:   %empty { $$ = NULL; }
                | words

parameters:     parameter { $$ = $1; $$->next = NULL; }
                | parameters parameter { $$ = $2; $2->next = $1; }

words:          WORD { $$ = $1; $$->next = NULL; }
                | words WORD {$$ = $2; $$->next = $1; }

parameter:      WORD '=' WORD {
  $$ = malloc(sizeof(struct parameter));
  $$->name = $1->w;
  $$->value = $3->w;
  free($1);
  free($3);
}

%%

extern FILE* yyin;

yyerror (char const *s)
{
	fprintf (stderr, "%s\n", s);
}
