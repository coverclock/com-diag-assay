/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assay.h"
#include "com/diag/diminuto/diminuto_containerof.h"

typedef enum AssayAction {
    NONE,
    ROOT,
    LEFT,
    RIGHT,
} assay_action_t;

/* HELPERS ********************************************************************/

static assay_section_t * assay_section_find_close(assay_section_t * scp, const char * section, int * rcp)
{
    *rcp = strcmp(scp->section, section);
    if (*rcp < 0) {
        if (!diminuto_tree_isleaf(scp->tree.right)) {
            scp = assay_section_find_close(diminuto_containerof(assay_section_t, tree, scp->tree.right), section, rcp);
        }
    } else if (*rcp > 0) {
        if (!diminuto_tree_isleaf(scp->tree.left)) {
            scp = assay_section_find_close(diminuto_containerof(assay_section_t, tree, scp->tree.left), section, rcp);
        }
    } else {
        /* Do nothing. */
    }

    return scp;
}

static assay_property_t * assay_property_find_close(assay_property_t * prp, const char * key, int * rcp)
{
    *rcp = strcmp(prp->key, key);
    if (*rcp < 0) {
        if (!diminuto_tree_isleaf(prp->tree.right)) {
            prp = assay_property_find_close(diminuto_containerof(assay_property_t, tree, prp->tree.right), key, rcp);
        }
    } else if (*rcp > 0) {
        if (!diminuto_tree_isleaf(prp->tree.left)) {
            prp = assay_property_find_close(diminuto_containerof(assay_property_t, tree, prp->tree.left), key, rcp);
        }
    } else {
        /* Do nothing. */
    }

    return prp;
}

/* CONFIG PRIMITIVES **********************************************************/

assay_config_t * assay_config_create(void)
{
    assay_config_t * cfp;

    cfp = (assay_config_t *)malloc(sizeof(assay_config_t));
    cfp->sections = DIMINUTO_TREE_EMPTY;

    return cfp;
}

assay_config_t * assay_config_load(assay_config_t * cfp, FILE * fp)
{
    return 0;
}

void assay_config_destroy(assay_config_t * cfp)
{
    diminuto_tree_t * streep;
    assay_section_t * scp;
    diminuto_tree_t * ptreep;
    assay_property_t * prp;

    for (streep = diminuto_tree_first(&(cfp->sections)); streep != DIMINUTO_TREE_NULL; streep = diminuto_tree_next(streep)) {
        scp = diminuto_containerof(assay_section_t, tree, streep);
        for (ptreep = diminuto_tree_first(&(scp->properties)); ptreep != DIMINUTO_TREE_NULL; ptreep = diminuto_tree_next(ptreep)) {
            prp = diminuto_containerof(assay_property_t, tree, ptreep);
            free((void *)(prp->key));
            free(prp->value);
            free(prp);
        }
        free((void *)(scp->section));
        free(scp);
    }
    free(cfp);
}

/* SECTION PRIMITIVES *********************************************************/

assay_section_t * assay_section_create(assay_config_t * cfp, const char * section)
{
    assay_section_t * scp;
    assay_section_t * tmp;
    int rc;
    assay_action_t action;

    if (diminuto_tree_isempty(&(cfp->sections))) {
        action = ROOT;
    } else {
        tmp = assay_section_find_close(diminuto_containerof(assay_section_t, tree, cfp->sections), section, &rc);
        if (rc < 0) {
            action = RIGHT;
        } else if (rc > 0) {
            action = LEFT;
        } else {
            action = NONE;
            scp = tmp;
        }
    }

    if (action != NONE) {

        scp = (assay_section_t *)malloc(sizeof(assay_section_t));
        diminuto_tree_init(&(scp->tree));
        scp->config = cfp;
        scp->section = strdup(section);
        scp->properties = DIMINUTO_TREE_EMPTY;

        switch (action) {
        case ROOT:
            diminuto_tree_insert_root(&(scp->tree), &(cfp->sections));
            break;
        case LEFT:
            diminuto_tree_insert_right(&(scp->tree), &(tmp->tree));
            break;
        case RIGHT:
            diminuto_tree_insert_left(&(scp->tree), &(tmp->tree));
            break;
        default:
            break;
        }

    }

    return scp;
}

assay_section_t * assay_section_seek(assay_config_t * cfp, const char * section)
{
    assay_section_t * scp;
    int rc;

    if (diminuto_tree_isempty(&(cfp->sections))) {
        scp = (assay_section_t *)0;
    } else {
        scp = assay_section_find_close(diminuto_containerof(assay_section_t, tree, cfp->sections), section, &rc);
        if (rc != 0) {
            scp = (assay_section_t *)0;
        }
    }

    return scp;
}

assay_section_t * assay_section_first(assay_config_t * cfp)
{
    assay_section_t * scp;
    diminuto_tree_t * treep;

    if ((treep = diminuto_tree_first(&(cfp->sections))) == DIMINUTO_TREE_NULL) {
        scp = (assay_section_t *)0;
    } else {
        scp = diminuto_containerof(assay_section_t, tree, treep);
    }

    return scp;
}

assay_section_t * assay_section_next(assay_section_t * scp)
{
    diminuto_tree_t * treep;

    if (scp == (assay_section_t *)0) {
        /* Do nothing. */
    } else if ((treep = diminuto_tree_next(&(scp->tree))) == DIMINUTO_TREE_NULL) {
        scp = (assay_section_t *)0;
    } else {
        scp = diminuto_containerof(assay_section_t, tree, treep);
    }

    return scp;
}

/* (There is deliberately no assay_section_destroy function. */

/* PROPERTY PRIMITIVES ********************************************************/

assay_property_t * assay_property_create(assay_section_t * scp, const char * key)
{
    assay_property_t * prp;
    assay_property_t * tmp;
    int rc;
    assay_action_t action;

    if (scp == (assay_section_t *)0) {
        action = NONE;
        prp = (assay_property_t *)0;
    } else if (diminuto_tree_isempty(&(scp->properties))) {
        action = ROOT;
    } else {
        tmp = assay_property_find_close(diminuto_containerof(assay_property_t, tree, scp->properties), key, &rc);
        if (rc < 0) {
            action = RIGHT;
        } else if (rc > 0) {
            action = LEFT;
        } else {
            action = NONE;
            prp = tmp;
        }
    }

    if (action != NONE) {

        prp = (assay_property_t *)malloc(sizeof(assay_property_t));
        diminuto_tree_init(&(prp->tree));
        prp->section = scp;
        prp->key = strdup(key);
        prp->value = 0;
        prp->length = 0;

        switch (action) {
        case ROOT:
            diminuto_tree_insert_root(&(prp->tree), &(scp->properties));
            break;
        case LEFT:
            diminuto_tree_insert_right(&(prp->tree), &(tmp->tree));
            break;
        case RIGHT:
            diminuto_tree_insert_left(&(prp->tree), &(tmp->tree));
            break;
        default:
            break;
        }

    }

    return prp;
}

assay_property_t * assay_property_seek(assay_section_t * scp, const char * key)
{
	assay_property_t * prp;
    int rc;

    if (scp == (assay_section_t *)0) {
        prp = (assay_property_t *)0;
    } else if (diminuto_tree_isempty(&(scp->properties))) {
        prp = (assay_property_t *)0;
    } else {
        prp = assay_property_find_close(diminuto_containerof(assay_property_t, tree, scp->properties), key, &rc);
        if (rc != 0) {
            prp = (assay_property_t *)0;
        }
    }

    return prp;
}

assay_property_t * assay_property_first(assay_section_t * scp)
{
    assay_property_t * prp;
    diminuto_tree_t * treep;

    if (scp == (assay_section_t *)0) {
        prp = (assay_property_t *)0;
    } else if ((treep = diminuto_tree_first(&(scp->properties))) == DIMINUTO_TREE_NULL) {
        prp = (assay_property_t *)0;
    } else {
        prp = diminuto_containerof(assay_property_t, tree, treep);
    }

    return prp;
}

assay_property_t * assay_property_next(assay_property_t * prp)
{
    diminuto_tree_t * treep;

    if (prp == (assay_property_t *)0) {
        /* Do nothing. */
    } else if ((treep = diminuto_tree_next(&(prp->tree))) == DIMINUTO_TREE_NULL) {
        prp = (assay_property_t *)0;
    } else {
        prp = diminuto_containerof(assay_property_t, tree, treep);
    }

    return prp;
}

void assay_property_destroy(assay_property_t * prp)
{
    if (prp != (assay_property_t *)0) {
        diminuto_tree_remove(&(prp->tree));
        free((void *)(prp->key));
        free(prp->value);
        free(prp);
    }
}

/* KEY PRIMITIVES *************************************************************/

const char * assay_key_get(assay_property_t * prp)
{
    const char * key;

    if (prp == (assay_property_t *)0) {
        key = (const char *)0;
    } else {
        key = prp->key;
    }

    return key;
}

/* VALUE PRIMITIVES ***********************************************************/

const char * assay_value_get(assay_property_t * prp, size_t * lengthp)
{
    const char * value;

    if (prp == (assay_property_t *)0) {
        value = (const char *)0;
    } if ((value = (const char *)prp->value) == (void *)0) {
        /* Do nothing. */
    } else if (lengthp == (size_t *)0) {
        /* Do nothing. */
    } else {
        *lengthp = prp->length;
    }

    return value;
}

assay_property_t * assay_value_set(assay_property_t * prp, const char * value, size_t length)
{
    if (prp != (assay_property_t *)0) {
        free(prp->value);
        prp->length = length;
        prp->value = malloc(prp->length);
        memcpy(prp->value, value, prp->length);
    }

    return prp;
}

/* CONFIG COMPOSITES **********************************************************/

const char * assay_config_get(assay_config_t * cfp, const char * section, const char * key)
{

}

const char * assay_config_set(assay_config_t * cfp, const char * section, const char * key, const char * value)
{

}
