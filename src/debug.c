#include "pool.h"
#include "dict.h"
#include "nfer.h"
#include "map.h"
#include "log.h"

unsigned int log_pool_use(const char *name, pool *p) {
    size_t bytes, map_bytes;
    pool_iterator pit;
    interval *i;
    unsigned int map_value_nodes;

    bytes = p->space * sizeof(interval_node);
    log_msg("Pool %s (%x) allocates %zu bytes for %u intervals.  %u are stored and %u are marked for removal.  ", name, p, bytes, p->space, p->size, p->removed);

    map_bytes = 0;
    // skip this if the pool is empty
    if (p->size > 0) {
        // iterate over the intervals in the pool and collect info about their maps, which are separately allocated
        map_value_nodes = 0;
        get_pool_iterator(p, &pit);
        while (has_next_interval(&pit)) {
            i = next_interval(&pit);
            map_value_nodes = map_value_nodes + i->map.space;
        }
        map_bytes = map_value_nodes * sizeof(map_value_node);
        log_msg("There are %u total map keys allocated using %zu bytes.\n", map_value_nodes, map_bytes);
    } else {
        log_msg("\n");
    }
    return p->space;
}

unsigned int log_dictionary_use(const char *name, dictionary *dict) {
    size_t bytes;

    bytes = dict->space * sizeof(word);
    log_msg("Dict %s (%x) allocates %zu bytes for %u words.  %u are stored.\n", name, dict, bytes, dict->space, dict->size);
    return dict->space;
}

void log_specification_use(dictionary *name_dict, nfer_specification *spec) {
    rule_id i;
    nfer_rule *rule;
    unsigned int exp_inputs, map_value_nodes, map_expressions, stack_values;
    map_iterator mit;
    map_key key;
    map_value entry;
    unsigned int rule_cache_size, largest_rule_cache, new_pool_size, largest_new_pool;

    // rules
    log_msg("%zu bytes allocated for %u rules.  %u are stored.\n", spec->space * sizeof(nfer_rule), spec->space, spec->size);
    // each rule...

    largest_rule_cache = 0;
    largest_new_pool = 0;

    for (i = 0; i < spec->size; i++) {
        exp_inputs = 0;
        map_value_nodes = 0;
        map_expressions = 0;
        stack_values = 0;
        
        rule = &spec->rules[i];
        log_msg("Rule producing %s: ", get_word(name_dict, rule->result_label));
        // expressions
        if (rule->where_expression != NULL) {
            exp_inputs = exp_inputs + rule->where_expression[0].length;
        }
        if (rule->begin_expression != NULL) {
            exp_inputs = exp_inputs + rule->begin_expression[0].length;
        }
        if (rule->end_expression != NULL) {
            exp_inputs = exp_inputs + rule->end_expression[0].length;
        }
        // map for map expressions
        map_value_nodes = map_value_nodes + rule->map_expressions.space;
        // the map expressions themselves
        get_map_iterator(&rule->map_expressions, &mit);
        while(has_next_map_key(&mit)) {
            key = next_map_key(&mit);
            map_get(&rule->map_expressions, key, &entry);
            if (entry.type == pointer_type) {
                map_expressions = map_expressions + ((expression_input *)(entry.value.pointer))[0].length;
            }
        }
        // stack
        stack_values = stack_values + rule->expression_stack.space;
        log_msg("%zu bytes for %u expressions. ", exp_inputs * sizeof(expression_input), exp_inputs);
        log_msg("%zu bytes for %u map keys. ", map_value_nodes * sizeof(map_value_node), map_value_nodes);
        log_msg("%zu bytes for %u map expressions. ", map_expressions * sizeof(expression_input), map_expressions);
        log_msg("%zu bytes for %u stack values.\n", stack_values * sizeof(stack_value), stack_values);
        // pools
        new_pool_size = log_pool_use("new_intervals", &rule->new_intervals);
        if (new_pool_size > largest_new_pool) {
            largest_new_pool = new_pool_size;
        }
        rule_cache_size = log_pool_use(get_word(name_dict, rule->left_label), &rule->left_cache);
        if (rule_cache_size > largest_rule_cache) {
            largest_rule_cache = rule_cache_size;
        }
        rule_cache_size = log_pool_use(get_word(name_dict, rule->right_label), &rule->right_cache);
        if (rule_cache_size > largest_rule_cache) {
            largest_rule_cache = rule_cache_size;
        }
        rule_cache_size = log_pool_use("produced", &rule->produced);
                rule_cache_size = log_pool_use(get_word(name_dict, rule->right_label), &rule->right_cache);
        if (rule_cache_size > largest_rule_cache) {
            largest_rule_cache = rule_cache_size;
        }
        // add a visual break
        log_msg("--\n");
    }
    log_msg("Suggested minimum memory configuration for compiled monitor:\n");
    log_msg("#define RULE_CACHE_SIZES %u\n", largest_rule_cache);
    log_msg("#define NEW_INTERVALS_SIZE %u\n", largest_new_pool);
}
