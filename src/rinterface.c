/*
 * rinterface.c
 *
 *  Created on: Feb 5, 2017
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

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
/* this is so we can use the R dynamic loading interface */
#include <R_ext/Rdynload.h>

#include <stdio.h>
#include <inttypes.h>

#include "types.h"
#include "dict.h"
#include "pool.h"
#include "nfer.h"
#include "log.h"
#include "dsl.h"
#include "strings.h"

#include "learn.h"

#define ROW_NAME_BUFFER_LENGTH 32

/**
 * Converts an R dataframe to an nfer pool.
 * Dataframes must have at least two columns but may have an arbitrary number >2.
 * The first two columns are used as name and timestamp.
 * Subsequent columns are used as data, where the column name is used as the 
 * map key and any non-NA values are used as map values.
 * 
 * Dataframe events:
 * | name | timestamp | x |    y |
 * -------------------------------
 * | foo  | 123       | 2 | NA   |
 * | bar  | 987       | 3 | TRUE |
 * 
 * results in p (representing in standard nfer event notation):
 * foo|123|x|2
 * bar|987|x;y|3;true
 * 
 * Name is assumed to a string (should be checked by calling R code).
 * Timestamp should be numeric or string as we can lose precision converting from floats.
 * Other fields can be any type, but be aware that they may not come out the other end the same.
 * 
 * The filter flag tells dataframe_to_pool to only keep data columns where
 * the name is already present in the key_dict.  If it is true, only already
 * present keys will be added to inteval maps.  If it is false, all columns
 * will be converted to keys and their column names will be added to key_dict.
 */
static void dataframe_to_pool(SEXP events, pool *p, dictionary *name_dict, 
                                dictionary *key_dict, dictionary *val_dict, bool filter) {
    int i, column;
    timestamp time_value;
    const char *name_value, *colname_value, *string_int;
    SEXP names_in, times_in, colnames, *data_columns;
    word_id name_id;
    interval *intv;
    R_xlen_t ncols;
    map_key *keys;
    map_value value;

    // initialize pointers to null
    data_columns = NULL;
    keys = NULL;

    // input validation is already done for names and times
    names_in = VECTOR_ELT(events, 0);
    times_in = VECTOR_ELT(events, 1);
    // now read data columns
    ncols = XLENGTH(events) - 2;
    // skip all of the map stuff if there's no extra columns
    if (ncols > 0) {
        // we have to allocate some space for the pointers
        data_columns = malloc(sizeof(SEXP) * ncols);
        // then for the map keys
        keys = malloc(sizeof(map_key) * ncols);
        if (data_columns == NULL || keys == NULL) {
            error("Could not allocate space for converting dataframes to pools.");
        }
        // get the names of the columns
        colnames = PROTECT(GET_NAMES(events));
        // read vectors for each column and store the column name, if not filtering
        for (column = 0; column < ncols; column++) {
            // get a pointer to the column name
            colname_value = CHAR(STRING_ELT(colnames, column + 2));
            
            // if filter is set, don't add keys or get the corresponding columns
            if (!filter || find_word(key_dict, colname_value) != WORD_NOT_FOUND) {
                filter_log_msg(LOG_LEVEL_DEBUG, "Adding column number %i: %s\n", column, colname_value);
                keys[column] = add_word(key_dict, colname_value);
                data_columns[column] = VECTOR_ELT(events, column + 2);
            } else {
                keys[column] = WORD_NOT_FOUND;
                data_columns[column] = NULL;
            }
        }
        // we can now unprotect colnames
        UNPROTECT(1);
    }

    for (i = 0; i < length(names_in); i++) {
        filter_log_msg(LOG_LEVEL_SUPERDEBUG, "Parsing row %i of %i\n", i, length(names_in));
        // get the strings, which requires first getting the CHARSXP
        name_value = CHAR(STRING_ELT(names_in, i));
        // timestamps can be integer, numeric, or string type
        switch(TYPEOF(times_in)) {
            case INTSXP:
                // integers are easy
                time_value = (timestamp) (INTEGER(times_in)[i]);
                break;
            case REALSXP:
                // numeric we have to deal with casting problems
                time_value = (timestamp) (REAL(times_in)[i]);
                break;
            case STRSXP:
                // for a string, we have to try to convert
                string_int = CHAR(STRING_ELT(times_in, i));
                // magic number alert - 20 is the number of digits in the largest 64 bit unsigned int
                // it's not clear if we can do better than this :(
                time_value = (timestamp) string_to_u64(string_int, MAX_64BIT_DIGITS);
                break;
            default:
                filter_log_msg(LOG_LEVEL_ERROR, "Error parsing input, timestamps must be integer, numeric, or character type\n");
                // bail - we'll get this error for every row
                return;
        }

        // filter says only add events that are already in the dictionary
        if (!filter || find_word(name_dict, name_value) != WORD_NOT_FOUND) {
            filter_log_msg(LOG_LEVEL_SUPERDEBUG, "Adding event %s at time %" PRIu64 "\n", name_value, time_value);

            // add to the dictionary (safe even if filter is true)
            name_id = add_word(name_dict, name_value);
            // add to the pool (no map support yet)
            intv = allocate_interval(p);
            intv->name = name_id;
            intv->start = time_value;
            intv->end = time_value;
            intv->hidden = false;

            // now, if there are data columns, set them
            for (column = 0; column < ncols; column++) {
                if (keys[column] != WORD_NOT_FOUND) {
                    // convert from the R types
                    switch(TYPEOF(data_columns[column])) {
                        case LGLSXP:
                            value.value.boolean = (LOGICAL(data_columns[column])[i]);
                            value.type = boolean_type;
                            break;
                        case INTSXP:
                            value.value.integer = (INTEGER(data_columns[column])[i]);
                            value.type = integer_type;
                            break;
                        case REALSXP:
                            value.value.real = (REAL(data_columns[column])[i]);
                            value.type = real_type;
                            break;
                        case STRSXP:
                            // and we have to add it to the val dict
                            value.value.string = add_word(val_dict, CHAR(STRING_ELT(data_columns[column], i)));
                            value.type = string_type;
                            break;
                        default:
                            value.value.pointer = NULL;
                            value.type = null_type;
                            filter_log_msg(LOG_LEVEL_ERROR, "Error parsing data column %s (col %d, row %d)\n", get_word(key_dict, keys[column]), column, i);
                        }
                    map_set(&intv->map, keys[column], &value);
                }
            }
        }
    }
    // make sure the pool is in order
    sort_pool(p);
    // free arrays we needed to track maps
    if (ncols > 0) {
        free(data_columns);
        free(keys);
    }
}

/**
 * Converts an nfer pool to an R dataframe.
 * Interval names go in the first column, which is string type.
 * The second and third columns are start and end timestamps, and they are 
 * numeric type (for now).
 * Any data in the map is added as additional columns, and intervals that 
 * don't have the map key get NA.
 * The number of key columns to add is a parameter to the function instead of 
 * being inferred from the key_dict.
 * 
 * Input pool p (representing in standard nfer interval output):
 * foo|123|456|x|2
 * bar|654|987|x;y|3;true
 * 
 * Dataframe output:
 * | name | start | end | x |    y |
 * -------------------------------
 * | foo  | 123   | 456 | 2 | NA   |
 * | bar  | 654   | 987 | 3 | TRUE |
 * 
 * The mapped_keys argument is an array of map keys of length num_keys.
 * The indices of mapped_keys correspond to the columns of the produced data frame, while the contents of
 * each cell contain the map_key that should go in that column.
 */
static void pool_to_dataframe(pool *p, dictionary *name_dict, 
                                dictionary *key_dict, dictionary *val_dict, 
                                map_key *mapped_keys, int num_keys, SEXP df) {
    int i, n, map_key_index;
    pool_iterator pit;
    interval *intv;
    SEXP names_out, start_out, end_out, *data_out;
    SEXP df_class, col_names, row_names, data_value;
    char row_name_buffer[ROW_NAME_BUFFER_LENGTH], data_string_buffer[MAX_64BIT_DIGITS+1], *result_name;
    map_value value;

    // number of results
    n = p->size;

    // set up objects to turn out into a data frame
    df_class = PROTECT(mkString("data.frame"));
    row_names = PROTECT(NEW_CHARACTER(n));
    col_names = PROTECT(NEW_CHARACTER(3 + num_keys));
    SET_STRING_ELT(col_names, 0, mkChar("name"));
    SET_STRING_ELT(col_names, 1, mkChar("start"));
    SET_STRING_ELT(col_names, 2, mkChar("end"));

    // allocate the output vectors
    names_out = PROTECT(NEW_CHARACTER(n));
    // numeric is not ideal at all here - it's not clear what the best choice is, though
    // the problem is that we want consistent output but we don't want to lose precision
    // probably the best choice is to make it configurable...
    start_out = PROTECT(NEW_NUMERIC(n));
    end_out = PROTECT(NEW_NUMERIC(n));

    // allocate space for SEXP pointers...
    if (num_keys > 0) {
        data_out = malloc(sizeof(SEXP) * num_keys);
        for (map_key_index = 0; map_key_index < num_keys; map_key_index++) {
            // everything is a character type for now
            data_out[map_key_index] = PROTECT(NEW_CHARACTER(n));
            // set up the column name
            SET_STRING_ELT(col_names, 3 + map_key_index, mkChar(get_word(key_dict, mapped_keys[map_key_index])));
        }
    }

    get_pool_iterator(p, &pit);
    i = 0;
    while (has_next_interval(&pit)) {
        intv = next_interval(&pit);

        // set the name string in the output
        result_name = get_word(name_dict, intv->name);
        SET_STRING_ELT(names_out, i, mkChar(result_name));

        // set the timestamps in the output
        REAL(start_out)[i] = (double) intv->start;
        REAL(end_out)[i] = (double) intv->end;

        // set the data columns, which are currently all char type
        for (map_key_index = 0; map_key_index < num_keys; map_key_index++) {
            map_get(&intv->map, mapped_keys[map_key_index], &value);
            switch(value.type) {
                case boolean_type:
                    if (value.value.boolean) {
                        data_value = mkChar("TRUE");
                    } else {
                        data_value = mkChar("FALSE");
                    }
                    SET_STRING_ELT(data_out[map_key_index], i, data_value);
                    break;
                case integer_type:
                    snprintf(data_string_buffer, MAX_64BIT_DIGITS + 1, "%" PRIi64, value.value.integer);
                    data_value = mkChar(data_string_buffer);
                    SET_STRING_ELT(data_out[map_key_index], i, data_value);
                    break;
                case real_type:
                    snprintf(data_string_buffer, MAX_64BIT_DIGITS + 1, "%lf", value.value.real);
                    data_value = mkChar(data_string_buffer);
                    SET_STRING_ELT(data_out[map_key_index], i, data_value);
                    break;
                case string_type:
                    data_value = mkChar(get_word(val_dict, value.value.string));
                    SET_STRING_ELT(data_out[map_key_index], i, data_value);
                    break;
                case pointer_type:
                    // shouldn't happen!
                    filter_log_msg(LOG_LEVEL_ERROR, "Pointer type data map found in results!\n");
                    /* falls through */
                case null_type:
                    // this means it wasn't set, which is fine
                    // if we don't set NA then it becomes empty string, which isn't that bad
                    // but we would rather have NA as it creates a clearer distinction between the two
                    SET_STRING_ELT(data_out[map_key_index], i, NA_STRING);
                    break;
            }
        }

        // set a row name for the data frame dimension
        snprintf(row_name_buffer, ROW_NAME_BUFFER_LENGTH, "%d", i);
        SET_STRING_ELT(row_names, i, mkChar(row_name_buffer));
        i++;
    }

    // set names
    SET_VECTOR_ELT(df, 0, names_out);
    // set start times
    SET_VECTOR_ELT(df, 1, start_out);
    // set end times
    SET_VECTOR_ELT(df, 2, end_out);
    // set data
    for (map_key_index = 0; map_key_index < num_keys; map_key_index++) {
        SET_VECTOR_ELT(df, 3+map_key_index, data_out[map_key_index]);
    }

    // turn it into a data frame
    SET_CLASS(df, df_class);
    SET_NAMES(df, col_names);
    // there's no SET_ROWNAMES
    SET_ATTR(df, R_RowNamesSymbol, row_names);

    if (num_keys > 0) {
        free(data_out);
    }
    UNPROTECT(6 + num_keys);
}

/**
 * Read the handle stored in R memory and get out the spec and dicts.
 * The R nfer handle contains pointers to allocated memory where the spec
 * and dicts are stored.
 * This function retrieves those pointers and sets the passed double
 * pointers to those pointers.
 * 
 * The function also has the side effect (for now) of setting the log
 * level and file, which are the other things stored in the handle.
 */
static void load_from_R_handle(SEXP handle,
        nfer_specification **spec,
        dictionary **name_dict,
        dictionary **key_dict,
        dictionary **val_dict) {

    SEXP loglevel_vector;
    int loglevel_value;

    // get the logging level into a C variable
    loglevel_vector = VECTOR_ELT(handle, 0);
    // any other configuration, like window size, would come from other indices of the handle

    // get the value
    loglevel_value = INTEGER(loglevel_vector)[0];

    // set the log level
    set_log_level(loglevel_value);

    // now get the handles from the attributes
    *spec = R_ExternalPtrAddr(getAttrib(handle, install("spec")));
    *name_dict = R_ExternalPtrAddr(getAttrib(handle, install("name_dict")));
    *key_dict = R_ExternalPtrAddr(getAttrib(handle, install("key_dict")));
    *val_dict = R_ExternalPtrAddr(getAttrib(handle, install("val_dict")));
}

/**
 * A finalizer to clean up the memory allocated for a specification.
 */
static void spec_finalizer(SEXP pointer) {
    if(!R_ExternalPtrAddr(pointer)) {
        return;
    }
    destroy_specification(R_ExternalPtrAddr(pointer));
    R_ClearExternalPtr(pointer);
}

/**
 * A finalizer to clean up the memory allocated for a dictionary.
 */
static void dict_finalizer(SEXP pointer) {
    if(!R_ExternalPtrAddr(pointer)) {
        return;
    }
    destroy_dictionary(R_ExternalPtrAddr(pointer));
    R_ClearExternalPtr(pointer);
}

/**
 * This function sets up an R handle for an nfer specification.
 * It allocates memory for the spec and dictionaries and
 * passes those pointers back to the calling function via
 * double pointer arguments.  It also sets the pointers on
 * the handle, as well as the log level argument.
 * There is a side effect also, that the log parameters are set.
 * 
 * Returns the SEXP of the handle.
 */
static SEXP initialize_R_nfer(SEXP loglevel,
        nfer_specification **spec,
        dictionary **name_dict,
        dictionary **key_dict,
        dictionary **val_dict) {

    SEXP handle, column_names, loglevel_handle,
         spec_handle, name_dict_handle, key_dict_handle, val_dict_handle;

    int loglevel_value;

    // get the parameter values into C variables
    loglevel_value = INTEGER_VALUE(loglevel);

    // set the log level and file
    set_log_level(loglevel_value);

    // allocate space on the heap for the data structures
    // this permits separate specs for separate handles
    *name_dict = malloc(sizeof(dictionary));
    *key_dict  = malloc(sizeof(dictionary));
    *val_dict  = malloc(sizeof(dictionary));
    *spec      = malloc(sizeof(nfer_specification));
    // check them all at once...
    if (*name_dict == NULL ||
        *key_dict  == NULL ||
        *val_dict  == NULL ||
        *spec      == NULL) {
        
        if (*name_dict != NULL) {
            free(*name_dict);
        }
        if (*key_dict != NULL) {
            free(*key_dict);
        }
        if (*val_dict != NULL) {
            free(*val_dict);
        }
        if (*spec != NULL) {
            free(*spec);
        }
        error("Could not allocate space for internal data structures!");
    }

    // initialize our data structures
    initialize_dictionary(*name_dict);
    initialize_dictionary(*key_dict);
    initialize_dictionary(*val_dict);
    initialize_specification(*spec, 0);

    // create a vector to return that has spots for the configuration details
    handle = PROTECT(NEW_LIST(2));

    // create R objects to store the values we want
    loglevel_handle = PROTECT(NEW_INTEGER(1));
    INTEGER(loglevel_handle)[0] = loglevel_value;
    column_names = PROTECT(NEW_CHARACTER(1)); // set to a higher number if this needs to store more
    SET_STRING_ELT(column_names, 0, mkChar("loglevel"));

    SET_VECTOR_ELT(handle, 0, loglevel_handle); // also set high indices on the handle vector
    SET_NAMES(handle, column_names);

    // set up R external pointers for all our data structures
    spec_handle = PROTECT(R_MakeExternalPtr(*spec, install("spec"), R_NilValue));
    R_RegisterCFinalizerEx(spec_handle, spec_finalizer, TRUE);
    name_dict_handle = PROTECT(R_MakeExternalPtr(*name_dict, install("name_dict"), R_NilValue));
    R_RegisterCFinalizerEx(name_dict_handle, dict_finalizer, TRUE);
    key_dict_handle = PROTECT(R_MakeExternalPtr(*key_dict, install("key_dict"), R_NilValue));
    R_RegisterCFinalizerEx(key_dict_handle, dict_finalizer, TRUE);
    val_dict_handle = PROTECT(R_MakeExternalPtr(*val_dict, install("val_dict"), R_NilValue));
    R_RegisterCFinalizerEx(val_dict_handle, dict_finalizer, TRUE);

    // add attributes to the handle
    setAttrib(handle, install("spec"), spec_handle);
    setAttrib(handle, install("name_dict"), name_dict_handle);
    setAttrib(handle, install("key_dict"), key_dict_handle);
    setAttrib(handle, install("val_dict"), val_dict_handle);

    UNPROTECT(7);

    return handle;
}

/**
 * Callable function from R that loads an nfer spec and returns a handle.
 * Takes the spec filename as a parameter, along with logging configuration.
 * Initializes the handle and sets up logging, then loads the spec.
 * 
 * Returns the R handle to the nfer specification.
 */
SEXP R_nfer(SEXP specfile, SEXP loglevel) {
    SEXP handle;
    nfer_specification *spec;
    dictionary *name_dict, *key_dict, *val_dict;
    bool loaded;
    const char *specfile_value;

    handle = PROTECT(initialize_R_nfer(loglevel,
            &spec, &name_dict, &key_dict, &val_dict));

    // get the parameter values into C variables
    specfile_value = CHAR(STRING_ELT(specfile, 0));

    // try to load the spec from the passed file
    loaded = load_specification(specfile_value, spec, name_dict, key_dict, val_dict);

    if (!loaded) {
        error("Unable to load specification!");
    }

    // unprotect handle
    UNPROTECT(1);

    return handle;
}

/**
 * Callable function from R that applies a loaded nfer spec to a trace.
 * Takes an R handle to an nfer spec as the first argument.
 * The second argument is a dataframe containing the event trace to process.
 * 
 * First, this loads the specification from the handle.
 * Then, it loads the passed dataframe into a pool.
 * It then calls nfer to apply the specification to the pool.
 * Then, it convers the output pool to a dataframe and returns it.
 */
SEXP R_nfer_apply(SEXP handle, SEXP events) {
    SEXP out;

    pool in_pool, out_pool;
    nfer_specification *spec;
    dictionary *name_dict, *key_dict, *val_dict;
    dictionary_iterator dit;
    map_key possible_key, *found_keys;
    int out_cols, found_index;

    // get the spec, etc. from the passed-in handle
    load_from_R_handle(handle, &spec, &name_dict, &key_dict, &val_dict);

    // initialize the pools
    initialize_pool(&in_pool);
    initialize_pool(&out_pool);

    filter_log_msg(LOG_LEVEL_STATUS, "Creating initial pool\n");
    // get the events from the input
    dataframe_to_pool(events, &in_pool, name_dict, key_dict, val_dict, true);
    if (should_log(LOG_LEVEL_DEBUG)) {
        log_msg("\nInitial pool:\n---------------------------\n");
        output_pool(&in_pool, name_dict, key_dict, val_dict, WRITE_LOGGING);
    }

    filter_log_msg(LOG_LEVEL_STATUS, "Applying the rules\n");

    // run the spec on the input events
    run_nfer(spec, &in_pool, &out_pool);

    // it is desirable to have idempotent behavior, so we need to clear all the caches
    // TODO: implement a way to reset a spec (clear all its caches)

    if (should_log(LOG_LEVEL_INFO)) {
        log_msg("\nResulting pool:\n---------------------------\n");
        output_pool(&out_pool, name_dict, key_dict, val_dict, WRITE_LOGGING);
    }

    // we have to determine the number of columns before we allocate space
    // this depends on how many data items might be produced by the spec
    // we know we need at least 3 for the name, begin, and end
    out_cols = 3;
    // allocate space for the keys we find -
    // allocate enough space for the maximum number, then just store the
    // ones we need.  This data structure will be used in pool_to_df to
    // set up the columns.  Of course we still don't know types.
    found_keys = malloc(sizeof(map_key) * key_dict->size);
    found_index = 0;

    get_dictionary_iterator(key_dict, &dit);
    while (has_next_word(&dit)) {
        // map_key is just a word_id, but let's make that explicit
        possible_key = (map_key) next_word(&dit);
        if (is_mapped(spec, possible_key)) {
            out_cols++;
            found_keys[found_index++] = possible_key;
        }
    }
    out = PROTECT(NEW_LIST(out_cols));

    filter_log_msg(LOG_LEVEL_STATUS, "Converting pool to dataframe\n");
    pool_to_dataframe(&out_pool, name_dict, key_dict, val_dict, found_keys, found_index, out);

    // tear down the nfer objects used
    destroy_pool(&in_pool);
    destroy_pool(&out_pool);

    UNPROTECT(1);

    return out;
}

/**
 * Callable function from R that learns an nfer spec from a trace.
 * The first argument is the dataframe containing the trace of events.
 * The second and third arguments are logging configuration.
 * 
 * The function initializes a new R handle to an nfer specification.
 * It then loads the trace dataframe into a pool.
 * It calls the nfer learner on that pool, currently using default parameters.
 * The resulting specification is stored in the R handle and returned.
 */
SEXP R_nfer_learn(SEXP events, SEXP loglevel) {
    SEXP handle;
    nfer_specification *spec;
    dictionary *name_dict, *key_dict, *val_dict;

    pool in_pool;

    handle = PROTECT(initialize_R_nfer(loglevel,
            &spec, &name_dict, &key_dict, &val_dict));

    // initialize the input pool
    initialize_pool(&in_pool);

    // get the events and put them into a pool
    dataframe_to_pool(events, &in_pool, name_dict, key_dict, val_dict, false);

    // initialize the spec but don't allocate any space for rules
    initialize_specification(spec, 0);

    // run the learner
    run_learner_on_pool(&in_pool, 1, name_dict, key_dict, val_dict, spec, DEFAULT_CONFIDENCE, DEFAULT_SUPPORT);

    // tear down the objects used for learning
    destroy_pool(&in_pool);

    // teardown logging
    stop_logging();

    // unprotect handle
    UNPROTECT(1);

    return handle;
}

/**
 * Register the dynamic interface for the nfer package.
 */
static const R_CallMethodDef callMethods[] = {
    {"load_", (DL_FUNC) &R_nfer, 2},
    {"apply_", (DL_FUNC) &R_nfer_apply, 2},
    {"learn_", (DL_FUNC) &R_nfer_learn, 2},
    {NULL, NULL, 0}
};

/**
 * Set up the call methods for the package and perform other initialization tasks.
 * This is called when the package is loaded into memory.
 */
void R_init_nfer(DllInfo *info) {
    R_registerRoutines(info,
                       NULL,  // C methods
                       callMethods, // Call methods
                       NULL,  // fortran methods
                       NULL); // external methods
    // don't search for other entry points
    R_useDynamicSymbols(info, FALSE);
    // force use of the symbols, don't allow looking them up from strings
    R_forceSymbols(info, TRUE);
}
