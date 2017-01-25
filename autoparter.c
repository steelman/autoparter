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

struct device
{
    const char *name;
    const char *path;
    void *requires;
};

const struct rule *autoparter_rules;
const char *mount_prefix;

static const char *rule_types[] = {
    [DEVICE] = "device",
    [LABEL] = "label",
    [PARTITION] = "partition",
    [FILESYSTEM] = "filesystem",
    [MOUNT] = "mount",
    [UNKNOWN_RULE] = "UNKNOWN",
};

int (*rule_check_functions[])(const struct rule*) = {
    [DEVICE] = device_check_rule,
    [LABEL] = label_check_rule,
    [PARTITION] = partition_check_rule,
    [FILESYSTEM] = fs_check_rule,
    [MOUNT] = mount_check_rule,
    [UNKNOWN_RULE] = NULL,
};

const char *
getrulename (enum rule_type t)
{
    if (t < 0 || t >= UNKNOWN_RULE)
        t = UNKNOWN_RULE;
    return rule_types[t];
}

enum rule_type
getruletype (const char *s)
{
    enum rule_type i;

    for (i = 0; i < UNKNOWN_RULE; i++)
        {
            if (rule_types[i] != NULL && strcmp (rule_types[i], s) == 0)
                break;
        }
    return i;
}

int (*getrulecheck(enum rule_type t))(const struct rule*)
{
    if (t < 0 || t >= UNKNOWN_RULE)
        t = UNKNOWN_RULE;
    return rule_check_functions[t];
}


int
device_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int path = 0;

    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("path", p->name) == 0)
                path = 1;
            else
                return 0;
        }
    return path;
}

int
label_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int type;
    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("type", p->name) == 0)
                type = 1;
            else if (strcmp ("clear", p->name) == 0)
                ;
            else
                return 0;
        }
    return type;
}

int
partition_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int type;
    int size;
    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("size", p->name) == 0)
                size = 1;
            else if (strcmp ("type", p->name) == 0)
                type = 1;
            else if (strcmp ("start", p->name) == 0)
                ;
            else
                return 0;
        }
    return (type && size);
}

int
fs_check_rule (const struct rule *r)
{
    return 0;
}

int
mount_check_rule (const struct rule *r)
{
    return 0;
}

void
dump_rule (const struct rule *r)
{
    printf("type: %s\nname: %s\n-----\n", getrulename(r->type), r->name);
}

int
main (int ac, char *argv[])
{
    const struct rule *r;
#if YYDEBUG
    yydebug = 1;
#endif
    yyparse (&autoparter_rules);

    printf("-----\n");
    for (r = autoparter_rules; r != NULL; r = r->next)
        {
            printf("checking… %s\n", (getrulecheck(r->type))(r) == 0 ? "ERROR" : "OK");
            dump_rule(r);
        }

    return 0;
}
