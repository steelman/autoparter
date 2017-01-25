/* -*- mode: c -*- */
%{
#include "autoparter.h"
#include <stdio.h>
#include <stdlib.h>

    static const char *rule_types[] = {
        [DEVICE] = "device",
        [LABEL] = "label",
        [PARTITION] = "partition",
        [FILESYSTEM] = "filesystem",
        [MOUNT] = "mount",
        [UNKNOWN_RULE] = "UNKNOWN",
    };

    static const char* getrulename(enum rule_type t) {
        if (t < 0 || t >= UNKNOWN_RULE)
            t = UNKNOWN_RULE;
        return rule_types[t];
    }

    static enum rule_type getruletype(const char* s) {
        enum rule_type i;

        for (i = 0; i < UNKNOWN_RULE; i++) {
            if (rule_types[i] != NULL &&
                strcmp(rule_types[i], s) == 0)
                break;
        }
        return i;
    }

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

rules:          rule { $$ = $1; $$->next = NULL; *rules_out = $$; }
                | rules rule { $$ = $2; $$->next = $1; *rules_out = $$; }

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
