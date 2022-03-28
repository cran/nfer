/*
 * static.c
 *
 *  Created on: Dec 10, 2021
 *      Author: skauffma
 *
 *   nfer - a system for inferring abstractions of event streams
 *   Copyright (C) 2021  Sean Kauffman
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

#include <assert.h>
#include <stdlib.h>

#include "types.h"
#include "ast.h"
#include "dsl.tab.h"
#include "dict.h"
#include "static.h"
#include "nfer.h"
#include "log.h"

/* if this is included in a test binary, include the testio header to overwrite malloc and free */
#ifdef TEST
    #include "testio.h"
#endif

/**
 * There are two options for inputs and they each have pros and cons
 * 1) AST from the DSL
 *   > This means that we have access to AST data structures
 *   > We can modify the AST relatively easily compared to, for example, RPC expressions
 *   > We don't need to modify this code if we change underlying rule data structures
 * 2) Generated rules
 *   > This means we can operate on rules from sources other than the DSL
 *   > We don't need to modify this code if we change the DSL/AST
 * 
 * The more typical choice, I think, is to operate on the AST and that is 
 * my preference right now.  The main reason is that there are currently only two
 * sources of rules: the DSL, and mining.  The rules from mining will not
 * be very interesting right now, so analyzing them is not useful.
 **/

/**
 * Compiles a list of rule nodes to be used as vertices in a directed graph.
 * The caller should pass a pointer to a rule_digraph_vertex pointer variable.
 * This may or may not be pre-allocated.
 * The count pointer should hold the number of elements of *vetices at call time.
 * It will be updated by the function if more elements are added to vertices.
 **/
#ifndef TEST
static
#endif
bool get_rule_vertices(ast_node *node, rule_digraph_vertex **vertices, unsigned int *count) {
    bool success;
    rule_digraph_vertex *temp_vertices;

    if (node == NULL) {
        return true;
    }

    /* preconditions : node type, non-null pointers */
    assert(node->type == type_rule || node->type == type_rule_list || node->type == type_module_list);
    assert(vertices != NULL);
    assert(count != NULL);

    success = true;

    switch (node->type) {
    case type_rule:
        if (*count == 0) {
            // no space has yet been allocated
            // allocate space for one word_id
            *vertices = malloc(sizeof(rule_digraph_vertex));
            if (*vertices == NULL) {
                success = false;
                break;
            }

        } else {
            // some space has already been allocated
            // allocate more space for produced word_ids
            temp_vertices = realloc(*vertices, sizeof(rule_digraph_vertex) * ((*count) + 1));
            if (temp_vertices == NULL) {
                success = false;
                break;
            }
            // copy the pointer on success, the old one is invalid
            *vertices = temp_vertices;
            
        }
        // get the rule
        (*vertices)[*count].rule = node;
        // set initial incoming to zero
        (*vertices)[*count].incoming = 0;
        // set removed to false
        (*vertices)[*count].removed = false;

        // increment the count
        (*count)++;

        break;
    case type_rule_list:
        /* recurse */
        success = success && get_rule_vertices(node->rule_list.head, vertices, count);
        success = success && get_rule_vertices(node->rule_list.tail, vertices, count);
        break;
    case type_module_list:
        /* If this module is imported, recurse.  Otherwise, skip it, it's not used. */
        if (node->module_list.imported) {
            success = success && get_rule_vertices(node->module_list.rules, vertices, count);
        }
        success = success && get_rule_vertices(node->module_list.tail, vertices, count);
        break;
    default:
        /* this should not be reachable */
        success = false;        
        break;
    }

    return success;
}


/**
 * Gets the edges in the rule digraph for the interval produced by the passed rule.
 * Edges will go from the rule that produces the interval to any that consume it.
 * The vertices should already have been generated.
 * The rule parameter is the pointer to the root rule of the tree this function explores.
 * The edges pointer should be valid but its target does not need to be preallocated.
 * The edge_count pointer should point to a valid count of the number of edges.
 * It will be updated when edges are added.
 **/
#ifndef TEST
static
#endif
bool get_rule_edges_for_aie(atomic_interval_expr_node *aie_node, 
                    rule_digraph_vertex *vertices, unsigned int vertex_count, 
                    rule_digraph_vertex *consumer_vertex, 
                    rule_digraph_edge **edges, unsigned int *edge_count) {
    bool success;
    unsigned int i;
    rule_digraph_vertex *producer_vertex;
    rule_digraph_edge *temp_edges;

    /* preconditions : not null pointers */
    assert(vertices != NULL || vertex_count == 0);
    assert(consumer_vertex != NULL);
    assert(edges != NULL);
    assert(edge_count != NULL);

    success = true;

    /* check to see if the consumed interval matches any produced ones from vertices */
    for (i = 0; i < vertex_count; i++) {
        producer_vertex = &vertices[i];

        if (aie_node->result_id == producer_vertex->rule->rule.result_id) {
            // there's a match, so generate a new edge
            if (*edge_count == 0) {
                // no space has yet been allocated
                // allocate space for one word_id
                *edges = malloc(sizeof(rule_digraph_edge));
                if (*edges == NULL) {
                    success = false;
                    break;
                }

            } else {
                // some space has already been allocated
                // allocate more space for produced word_ids
                temp_edges = realloc(*edges, sizeof(rule_digraph_edge) * ((*edge_count) + 1));
                if (temp_edges == NULL) {
                    success = false;
                    break;
                }
                // copy the pointer on success, the old one is invalid
                *edges = temp_edges;
                
            }
            // set the values for the edge
            (*edges)[*edge_count].from = producer_vertex;
            (*edges)[*edge_count].to = consumer_vertex;
            // set removed to false
            (*edges)[*edge_count].removed = false;
            // increment incoming
            consumer_vertex->incoming++;

            // increment the count
            (*edge_count)++;
        }
    }

    return success;
}


/**
 * Gets the edges in the rule digraph.
 * Edges will go from the rule that produces the interval to any that consume it.
 * The vertices should already have been generated and passed in.
 * The rule parameter is the pointer to the root rule of the tree this function explores.
 * The edges pointer should be valid but its target does not need to be preallocated.
 * The edge_count pointer should point to a valid count of the number of edges.
 * It will be updated when edges are added.
 **/
#ifndef TEST
static
#endif
bool get_rule_edges(ast_node *node, 
                    rule_digraph_vertex *vertices, unsigned int vertex_count, 
                    rule_digraph_vertex *rule, 
                    rule_digraph_edge **edges, unsigned int *edge_count) {
    bool success;
    unsigned int i;
    rule_digraph_vertex *found_vertex;

    if (node == NULL) {
        return true;
    }

    /* preconditions : node type, non-null pointers */
    assert(node->type == type_module_list ||
           node->type == type_rule_list ||
           node->type == type_rule || 
           node->type == type_binary_interval_expr || 
           node->type == type_atomic_interval_expr);
    assert(vertices != NULL || vertex_count == 0);
    assert(edges != NULL);
    assert(edge_count != NULL);

    success = true;
    found_vertex = NULL;

    switch (node->type) {
    case type_atomic_interval_expr:
        success = get_rule_edges_for_aie(&node->atomic_interval_expr, vertices, vertex_count, rule, edges, edge_count);
        break;
    case type_binary_interval_expr:
        /* just recurse on a BIE */
        success = success && get_rule_edges(node->binary_interval_expr.left, vertices, vertex_count, rule, edges, edge_count);
        success = success && get_rule_edges(node->binary_interval_expr.right, vertices, vertex_count, rule, edges, edge_count);
        break;
    case type_rule:
        /* this is a rule!  call with the rule set */

        // first find the vertex for this rule
        for (i = 0; i < vertex_count; i++) {
            if (node == vertices[i].rule) {
                found_vertex = &vertices[i];
            }
        }
        success = success && get_rule_edges(node->rule.interval_expr, vertices, vertex_count, found_vertex, edges, edge_count);
        break;
    case type_rule_list:
        /* recurse to find all the rules */
        success = success && get_rule_edges(node->rule_list.head, vertices, vertex_count, NULL, edges, edge_count);
        success = success && get_rule_edges(node->rule_list.tail, vertices, vertex_count, NULL, edges, edge_count);
        break;
    case type_module_list:
        /* If this module is imported, recurse.  Otherwise, skip it, it's not used. */
        if (node->module_list.imported) {
            success = success && get_rule_edges(node->module_list.rules, vertices, vertex_count, NULL, edges, edge_count);
        }
        success = success && get_rule_edges(node->module_list.tail, vertices, vertex_count, NULL, edges, edge_count);
        break;
    default:
        /* this should not be reachable */
        success = false;        
        break;
    }

    return success;
}

/**
 * Checks for a cycle in the computed rule digraph.
 * Implements Kahn's algorithm for a topological sort of the rules.
 * Returns the vertices ordered according to the sort, or NULL if a cycle is present.
 **/
#ifndef TEST
static
#endif
ast_node * topological_sort(rule_digraph_vertex *vertices, unsigned int vertex_count, rule_digraph_edge *edges, unsigned int edge_count) {
    unsigned int i, j;
    ast_node *first_rule_list, *current_rule_list, *temp_rule_list;
    rule_digraph_vertex *current_vertex, *following_vertex;
    rule_digraph_edge *current_edge;

    /* preconditions: vertices not null, not zero */
    assert(vertices != NULL);
    assert(vertex_count > 0);

    // initialize the variables used for tracking the list
    first_rule_list = NULL;
    current_rule_list = NULL;

    // iterate over the list over vertices
    for (i = 0; i < vertex_count; i++) {
        // get a pointer for easier access
        current_vertex = &vertices[i];

        // if we have a vertex that is both not removed and has a zero incoming edge count
        if (current_vertex->incoming == 0 && !current_vertex->removed) {
            // remove it
            current_vertex->removed = true;
            // allocate a new rule list
            filter_log_msg(LOG_LEVEL_SUPERDEBUG, "    -- Leaf rule %x found producing %d\n", current_vertex->rule, current_vertex->rule->rule.result_id);
            // we have to build the list of rules backwards
            // current points to the end
            if (current_rule_list == NULL) {
                // we haven't created one yet, so make a new rule list and then set first to it so we can return it
                current_rule_list = new_rule_list(current_vertex->rule, NULL);
                first_rule_list = current_rule_list;
            } else {
                // current points to the end and tail is NULL
                temp_rule_list = new_rule_list(current_vertex->rule, NULL);
                current_rule_list->rule_list.tail = temp_rule_list;
                // keep current at the end
                current_rule_list = temp_rule_list;
            }

            // now iterate over the edges and look for any from current
            for (j = 0; j < edge_count; j++) {
                // get a handle
                current_edge = &edges[j];

                if (current_edge->from == current_vertex && !current_edge->removed) {
                    filter_log_msg(LOG_LEVEL_SUPERDEBUG, "    ---- Edge found for %d\n", current_vertex->rule->rule.result_id);

                    // first, remove the edge
                    current_edge->removed = true;
                    // get the vertex the edge points to
                    following_vertex = current_edge->to;
                    // decrement its incoming count
                    following_vertex->incoming--;
                    // if the number of incoming edges is zero, it can be inserted
                    // reset the iterator on the vertex loop to 0 (it will be incremented after the loop)
                    if (following_vertex->incoming == 0) {
                        filter_log_msg(LOG_LEVEL_SUPERDEBUG, "    ---- No more incoming edges for rule %x\n", following_vertex->rule);
                        i = -1;
                    }
                }
            }
        }
    }

    // now go through the list of vertices again and check if they have all been removed
    for (i = 0; i < vertex_count; i++) {
        // if one has not been removed, there is a cycle
        if (!vertices[i].removed) {
            // first, get rid of the rule lists we allocated
            current_rule_list = first_rule_list;
            while (current_rule_list != NULL) {
                temp_rule_list = current_rule_list->rule_list.tail;
                free(current_rule_list);
                current_rule_list = temp_rule_list;
            }
            // then return null
            return NULL;
        }
    }

    return first_rule_list;
}

/**
 * Determine if there are any exclusion rules in the specification.
 * Just recurses down the AST.  If there is an exclusive rule, returns true.
 * If no recurse rule exists, returns false.
 **/
#ifndef TEST
static
#endif
bool check_has_exclusion(ast_node *node) {
    bool found;

    // if we hit a null pointer (end of a list), return
    if (node == NULL) {
        return false;
    }

    /* precondition : just the node type */
    assert(node->type == type_module_list ||
           node->type == type_rule_list ||
           node->type == type_rule ||
           node->type == type_binary_interval_expr ||
           node->type == type_atomic_interval_expr);

    found = false;
    switch (node->type) {
    case type_atomic_interval_expr:
        /* do nothing - we just want to return */
        break;
    case type_binary_interval_expr:
        /* check for exclusions */
        if (node->binary_interval_expr.exclusion) {
            found = true;
        } else {

            found = check_has_exclusion(node->binary_interval_expr.left) ||
                    check_has_exclusion(node->binary_interval_expr.right);
        }
        break;
    case type_rule:
        found = check_has_exclusion(node->rule.interval_expr);
        break;
    case type_rule_list:
        found = check_has_exclusion(node->rule_list.head) ||
                check_has_exclusion(node->rule_list.tail);
        break;
    case type_module_list:
        /* skip any non-imported modules */
        if (node->module_list.imported) {
            found = check_has_exclusion(node->module_list.rules);
        }
        found = found || check_has_exclusion(node->module_list.tail);
        break;
    default:
        /* do nothing */
        break;
    }
    return found;
}

/**
 * Determine if any rule defines custom begin or end timestamps that refer to anything
 * apart from the four input begin and end timestamps.  This is strictly not permitted
 * if one relies on the known complexity classes.
 **/
#ifndef TEST
static
#endif
bool check_computes_ts(ast_node *node) {
    bool does_compute_ts;

    // if we hit a null pointer (end of a list), return
    if (node == NULL) {
        return false;
    }

    /* no preconditions - there are too many node types to make it worth checking */

    // doesn't do it unless we find otherwise
    does_compute_ts = false;

    switch (node->type) {
    case type_time_field:
        // users are allowed to specify end points are just a timestamp from the input
        does_compute_ts = false;
        break;
    case type_end_points:
        does_compute_ts = check_computes_ts(node->end_points.begin_expr) ||
                          check_computes_ts(node->end_points.end_expr);
        break;
    case type_rule:
        does_compute_ts = check_computes_ts(node->rule.end_points) ||
                          check_computes_ts(node->rule.map_expr_list);

        break;
    case type_rule_list:
        does_compute_ts = check_computes_ts(node->rule_list.head) ||
                          check_computes_ts(node->rule_list.tail);
        break;
    case type_module_list:
        /* skip any non-imported modules */
        if (node->module_list.imported) {
            does_compute_ts = check_computes_ts(node->module_list.rules);
        }
        does_compute_ts = does_compute_ts || check_computes_ts(node->module_list.tail);
        break;
    default:
        // if we get here, it's because we hit some arithmetic expression in the end points
        // therefore, we got some computation
        does_compute_ts = true;
        filter_log_msg(LOG_LEVEL_DEBUG, "    Found possible arithmetic in end point expression: %d\n", (int)node->type);
        break;
    }
    return does_compute_ts;
}


/**
 * Removes modules, rule_lists, and imports from the AST without destroying rules
 * and their associated children.  This is done so that the new rule_list created in
 * the topological sort can be used instead.
 **/
#ifndef TEST
static
#endif
void constrict_modules(ast_node *node) {
    if (node == NULL) {
        return;
    }

    /* technically we need to remove import lists... */
    assert(node->type == type_module_list || node->type == type_rule_list || node->type == type_import_list);
    
    switch (node->type) {
    case type_module_list:
        /* first remove all the children */
        constrict_modules(node->module_list.rules);
        constrict_modules(node->module_list.tail);
        constrict_modules(node->module_list.imports);

        /* then free the node */
        filter_log_msg(LOG_LEVEL_DEBUG, "  Freeing module_list %x\n", node);
        free(node);

        break;
    case type_import_list:
        /* just get rid of it */
        constrict_modules(node->import_list.tail);
        filter_log_msg(LOG_LEVEL_DEBUG, "    Freeing import list %x\n", node);
        free(node);

        break;

    case type_rule_list:
        /* get rid of following rule_lists first */
        constrict_modules(node->rule_list.tail);
        filter_log_msg(LOG_LEVEL_DEBUG, "    Freeing rule_list %x\n", node);
        free(node);

        break;
    default:
        /* should be unreachable */
        break;
    }
}

/**
 * Helper function to reset an analysis result.
 **/
void initialize_analysis(spec_analysis *analysis) {
    analysis->computes_ts = false;
    analysis->has_cycle = false;
    analysis->has_exclusion = false;
}

/**
 * Recurses over the AST and calls the various static analysis functions.
 * This is a parent function calls a bunch of smaller analysese that each may recurse over the AST.
 * Trying to put all of them together ends up with a mess, so this keeps things logically separate.
 **/
bool do_static_analysis(ast_node **root, spec_analysis *result) {
    bool success;
    rule_digraph_vertex *vertices;
    unsigned int vertex_count;
    rule_digraph_edge *edges;
    unsigned int edge_count;
    ast_node *sorted_rule_list;

    success = true;

    /**
     * Look for cycles and reorder rules if there are none
     * Algorithm is:
     *   1) get all the rule nodes - these are the vertices
     *   2) get all the edges - for each rule, walk its children
     *   3) perform a topological sort - if it is possible there's no cycle
     **/

    // get the vertices
    vertices = NULL;
    vertex_count = 0;
    filter_log_msg(LOG_LEVEL_DEBUG, "    Getting rule vertices for DIGRAPH...\n");
    success = success && get_rule_vertices(*root, &vertices, &vertex_count);

    // get the edges
    edges = NULL;
    edge_count = 0;
    filter_log_msg(LOG_LEVEL_DEBUG, "    Getting rule edges for DIGRAPH...\n");
    success = success && get_rule_edges(*root, vertices, vertex_count, NULL, &edges, &edge_count);

    filter_log_msg(LOG_LEVEL_DEBUG, "    Performing topological sort on rule DIGRAPH...\n");
    sorted_rule_list = topological_sort(vertices, vertex_count, edges, edge_count);
    if (sorted_rule_list != NULL) {
        filter_log_msg(LOG_LEVEL_INFO, "    No cycle found in spec.\n");
        // no cycles - we have a sort
        // clean out all the old modules/rule_lists from the AST
        constrict_modules(*root);

        // update the root pointer to the sorted rule list
        *root = sorted_rule_list;

        // make sure to update the analysis result
        result->has_cycle = false;

    } else {
        filter_log_msg(LOG_LEVEL_INFO, "    Cycle found in spec.\n");
        result->has_cycle = true;
    }

    /**
     * Cleanup for the cycle stuff.
     **/
    if (vertex_count > 0) {
        free(vertices);
    }
    if (edge_count > 0) {
        free(edges);
    }

    /* compute exclusion and timestamps only if we succeeded so far */
    if (success) {
        result->has_exclusion = check_has_exclusion(*root);
        if (result->has_exclusion) {
            filter_log_msg(LOG_LEVEL_INFO, "    Exclusive rules found.\n");
        } else {
            filter_log_msg(LOG_LEVEL_INFO, "    No exclusion rules found.\n");
        }
        result->computes_ts = check_computes_ts(*root);
        if (result->computes_ts) {
            filter_log_msg(LOG_LEVEL_INFO, "    Rules could compute new timestamps.\n");
        } else {
            filter_log_msg(LOG_LEVEL_INFO, "    Rules do not compute new timestamps.\n");
        }
    }

    return success;
}
