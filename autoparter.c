/* -*- mode: c -*- */
/* Copyright (C) 2017 Łukasz Stelmach
 *
 * This file is part of autoparter.
 * 
 * Autoparter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#define _GNU_SOURCE
#include <stdio.h>
#include "parser.h"
#include "autoparter.h"

extern FILE *yyin;

struct device {
    const char* name;
    const char* path;
    void *requires;
};

const struct rule *autoparter_rules;

int device_check_rule(const struct rule *r)
{
    const struct parameter *p;
    int path=0;

    for(p = r->parameters; p != NULL; p = p->next) {
        if (strcmp("path", p->name) == 0)
            path = 1;
        else
            return 1;
    }
    return 0;
}

int main(int ac, char *argv[])
{
#if YYDEBUG
    yydebug=1;
#endif
    do {
        yyparse(&autoparter_rules);
        printf("-----\n");
    } while (!feof(yyin));
    return 0;
}
