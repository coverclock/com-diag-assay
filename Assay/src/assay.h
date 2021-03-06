/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_PRIVATE_
#define _H_COM_DIAG_ASSAY_PRIVATE_

/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.
 *
 * This describes the Assay private API.
 */

#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_tree.h"

#define ASSAY_BUFFER_DEFAULT_SIZE (64)

/**
 * This structure contains a buffer used during parsing.
 */
typedef struct AssayAction {
    char * buffer;
    size_t length;
    size_t index;
} assay_action_t;

/**
 * This structure contains the fields of a property.
 */
struct AssayProperty {
    diminuto_tree_t     tree;
    assay_section_t *   section;
    const char *        key;
    void *              value;
    size_t              length;
};

/**
 * This structure contains the fields of a section.
 */
struct AssaySection {
    diminuto_tree_t     tree;
    assay_config_t *    config;
    const char *        name;
    diminuto_tree_t *   properties;
};

/**
 * This structure contains the fields of a configuration.
 */
struct AssayConfig {
    diminuto_tree_t *   sections;
    assay_section_t *   section;
    assay_property_t *  property;
    assay_action_t      oaction;
    assay_action_t      aaction;
    assay_action_t      saction;
    assay_action_t      kaction;
    assay_action_t      vaction;
    FILE *              stream;
    const char *        file;
    int                 lines;
    int                 errors;
};

/**
 * Increment the error counter in the configuration.
 * @param cfp points to the configuration.
 * @return the current error counter.
 */
extern int assay_config_error(assay_config_t * cfp);


/**
 * Increment the line counter in the configuration.
 * @param cfp points to the configuration.
 * @return the current line counter.
 */
extern int assay_config_line(assay_config_t * cfp);

#endif
