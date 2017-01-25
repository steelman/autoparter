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

enum rule_type {
    DEVICE = 0,
    LABEL,
    PARTITION,
    FILESYSTEM,
    MOUNT,
    UNKNOWN_RULE,
};

struct parameter
{
    const char* name;
    const char* value;
    const struct parameter *next;
};

struct word
{
    const char* w;
    const struct word *next;
};

struct rule
{
    enum rule_type type;
    const char* name;
    const struct parameter *parameters;
    const struct word *prerequisites;
    const struct rule *next;
};

const char *getrulename (enum rule_type);
enum rule_type getruletype (const char *);
int device_check_rule (const struct rule*);
int label_check_rule (const struct rule*);
int partition_check_rule (const struct rule*);
int fs_check_rule (const struct rule*);
int mount_check_rule (const struct rule*);
