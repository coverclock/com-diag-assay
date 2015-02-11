//* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_PRIVATE_
#define _H_COM_DIAG_ASSAY_PRIVATE_

/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_tree.h"

struct AssayProperty {
    diminuto_tree_t     tree;
    assay_section_t *   section;
    const char *        key;
    void *              value;
    size_t              length;
};

struct AssaySection {
    diminuto_tree_t     tree;
    assay_config_t *    config;
    const char *        section;
    diminuto_tree_t *   properties;
};

struct AssayConfig {
    diminuto_tree_t *   sections;
};

#endif
