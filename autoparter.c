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
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "autoparter.h"

extern FILE *yyin;

struct rule *autoparter_rules;
struct rule *sorted_rules = NULL;
const char *mount_prefix;
int autoparter_lineno;
const char* autoparter_file = "stdin";

static const char *rule_types[] = {
    [DEVICE] = "device",
    [LABEL] = "label",
    [PARTITION] = "partition",
    [FILESYSTEM] = "filesystem",
    [MOUNT] = "mount",
    [UNKNOWN_RULE] = "UNKNOWN",
};

static const size_t rule_sizes[] = {
    [DEVICE] = sizeof(struct device_rule),
    [LABEL] = sizeof(struct label_rule),
    [PARTITION] = sizeof(struct partition_rule),
    [FILESYSTEM] = sizeof(struct fs_rule),
    [MOUNT] = sizeof(struct mount_rule),
    [UNKNOWN_RULE] = 0,
};

int (*rule_check_functions[])(const struct rule*) = {
    [DEVICE] = device_check_rule,
    [LABEL] = label_check_rule,
    [PARTITION] = partition_check_rule,
    [FILESYSTEM] = fs_check_rule,
    [MOUNT] = mount_check_rule,
    [UNKNOWN_RULE] = NULL,
};

int (*rule_ready_functions[])(const struct rule*) = {
    [DEVICE] = device_ready_p,
    [LABEL] = label_ready_p,
    [PARTITION] = partition_ready_p,
    [FILESYSTEM] = fs_ready_p,
    [MOUNT] = mount_ready_p,
    [UNKNOWN_RULE] = NULL,
};

const struct rule *
lookup_rule(const char* n, const struct rule* r)
{
    const struct rule* p;
    for (p = r; p != NULL; p = p-> next)
        {
            if (strcmp(n, p->name) == 0)
                return p;
        }
    return NULL;
}

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

int (*getrulecheck (enum rule_type t)) (const struct rule *)
{
    if (t < 0 || t >= UNKNOWN_RULE)
        t = UNKNOWN_RULE;
    return rule_check_functions[t];
}

int (*getruleready (enum rule_type t)) (const struct rule *)
{
    if (t < 0 || t >= UNKNOWN_RULE)
        t = UNKNOWN_RULE;
    return rule_ready_functions[t];
}

/* ---------------------------------------------------------------- */
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

int device_ready_p (const struct rule *r)
{
    if (
    return 0;
}

/* ---------------------------------------------------------------- */
int
label_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int ck_type;
    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("type", p->name) == 0)
                ck_type = 1;
            else if (strcmp ("clear", p->name) == 0)
                ;
            else
                return 0;
        }
    return !!ck_type;
}

int
label_ready_p (const struct rule *r)
{
    return 0;
}

/* ---------------------------------------------------------------- */
int
partition_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int ck_type;
    int ck_size;
    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("size", p->name) == 0)
                ck_size = 1;
            else if (strcmp ("type", p->name) == 0)
                ck_type = 1;
            else if (strcmp ("start", p->name) == 0)
                ;
            else
                return 0;
        }
    return (ck_type && ck_size);
}

int
partition_ready_p (const struct rule *r)
{
    return 0;
}
/* ---------------------------------------------------------------- */
int
fs_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int ck_type;
    int ck_uuid;
    int ck_format;

    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("type", p->name) == 0)
                ck_type = 1;
            else if (strcmp ("uuid", p->name) == 0)
                ck_uuid = 1;
            else if (strcmp ("format", p->name) == 0 &&
                     (p->value[0] == 'y' ||  p->value[0] == 'Y'))
                ck_format = 1;
            else
                return 0;
        }
    return ((!ck_type && ck_uuid && !ck_format) ||
            (ck_type && !ck_uuid && ck_format && (r->prerequisites)));
}

int
fs_ready_p (const struct rule *r)
{
    /* Try to find the FS by uuid */
    return 0;
}

/* ---------------------------------------------------------------- */
int
mount_check_rule (const struct rule *r)
{
    const struct parameter *p;
    int ck_target;
    for (p = r->parameters; p != NULL; p = p->next)
        {
            if (strcmp ("target", p->name) == 0)
                ck_target = 1;
            else if (strcmp ("options", p->name) == 0)
                ;
            else
                return 0;
        }
    return !!ck_target;
}

int
mount_ready_p (const struct rule *r)
{
    return 0;
}

/* ---------------------------------------------------------------- */
void
topo_visit (struct rule *n, struct rule ***last)
{
    if (n->tmark)
        {
            fprintf(stderr, "cyclic dependency detected for rule \"%s\"\n",
                    n->name);
            exit(1);
        }

    if (!n->pmark)
        {
            const struct word *w;
            struct rule *t;
            n->tmark = 1;
            for (w = n->prerequisites; w != NULL; w = w->next)
                {
                    struct rule *m;
                    if ((m = (struct rule*)lookup_rule(w->w, autoparter_rules)) == NULL)
                        {
                            fprintf(stderr, "prerequisite \"%s\""
                                    " for rule \"%s\" not found\n",
                                    w->w, n->name);
                            exit(1);
                        }
                    topo_visit(m, last);
                }
            n->pmark = 1;
            n->tmark = 0;

            t = malloc(sizeof(struct rule));
            t = memcpy(t, n, sizeof(struct rule));
            t->next = NULL;

            /* Add at the end of the list */
            **last = t;
            *last = (struct rule**)&(t->next);
        }
}

void
topo_sort (const struct rule *r, struct rule **s)
{
    struct rule *t;
    struct rule **last = s;

    for (t = (struct rule*)r;
         t != NULL;
         t = (struct rule*)t->next)
        {
            if (t->pmark)
                continue;
            topo_visit(t, &last);
            t = (struct rule*)r;
        }
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
    autoparter_lineno = 1;
    yyparse ();

    topo_sort(autoparter_rules, &sorted_rules);

    printf("-----\n");
    for (r = sorted_rules; r != NULL; r = r->next)
        {
            printf("checking… %s\n",
                   (getrulecheck(r->type))(r) == 0 ? "ERROR" : "OK");

            if (r->prerequisites == NULL)
                {
                    printf("no prerequisites, is the object ready? %s\n",
                           (getruleready(r->type))(r) == 0 ? "NO" : "YES");
                }

            dump_rule(r);
        }
    return 0;
}
