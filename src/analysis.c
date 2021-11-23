/*
 * analysis.c
 *
 *  Created on: Jun 2, 2017
 *      Author: skauffma
 *
 *    nfer - a system for inferring abstractions of event streams
 *   Copyright (C) 2017  Sean Kauffman
 *
 *   This file is part of nfer.
 *   nfer is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "dict.h"
#include "nfer.h"
#include "log.h"
#include "analysis.h"
#include "memory.h"


void log_event_groups(nfer_specification *spec, dictionary *name_dict) {
    int i, j, new_group_num, *group_assignments, *group_counts, num_labels;
    rule_id id;
    nfer_rule *rule;
    bool first;

    if (spec->size > 0) {
        filter_log_msg(LOG_LEVEL_DEBUG, "Number of rules %d\n", spec->size);

        // find the number of subscribed names
        num_labels = name_dict->size;

        // find event groups
        group_assignments = malloc(sizeof(int) * num_labels);
        clear_memory(group_assignments, sizeof(int) * num_labels);
        // can't have more groups than num_labels
        group_counts = malloc(sizeof(int) * num_labels);
        clear_memory(group_counts, sizeof(int) * num_labels);
        new_group_num = 0;

        // iterate over the rules
        for (id = 0; id < spec->size; id++) {
            // get the rule
            rule = &spec->rules[id];

            // if neither left or right side has been assigned a group
            // then pick a new group number and assign it to both
            if (group_assignments[rule->left_label] == 0 && group_assignments[rule->right_label] == 0) {
                new_group_num++;
                group_assignments[rule->left_label]  = new_group_num;
                group_assignments[rule->right_label] = new_group_num;
                group_counts[new_group_num - 1] = 1;
            } else {
                // otherwise, at least one of them is assigned a group alread so copy that

                // mark right side
                if (group_assignments[rule->right_label] == 0) {
                    group_assignments[rule->right_label] = group_assignments[rule->left_label];
                    group_counts[group_assignments[rule->left_label] - 1]++;
                }

                // mark left side
                if (group_assignments[rule->left_label] == 0) {
                    group_assignments[rule->left_label] = group_assignments[rule->right_label];
                    group_counts[group_assignments[rule->right_label] - 1]++;
                }
            }
        }

        for (j = 1; j <= new_group_num; j++) {
            if (group_counts[j - 1] > 1) {
                log_msg("Group %d:", j);
                first = true;

                for (i = 0; i < num_labels; i++) {
                    if (group_assignments[i] == j) {
                        if (!first) {
                            log_msg(",");
                        }
                        log_msg(" %s", get_word(name_dict, i));
                        first = false;
                    }
                }
                log_msg("\n");
            }
        }
        free(group_assignments);
        free(group_counts);
    }
}
