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
#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_tree.h"
#include "com/diag/diminuto/diminuto_containerof.h"
#include "com/diag/diminuto/diminuto_log.h"

/*******************************************************************************
 * TYPES
 ******************************************************************************/

struct AssayProperty {
    diminuto_tree_t     tree;
    assay_section_t *   section;
    const char *        key;
    const void *        value;
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
    assay_property_t *  cache;
};

typedef enum AssayAction {
    NONE,
    ROOT,
    LEFT,
    RIGHT,
} assay_action_t;

/*******************************************************************************
 * CONFIGURATION PRIMITIVES
 ******************************************************************************/

assay_config_t * assay_config_create(void)
{
    assay_config_t * cfp;

    cfp = (assay_config_t *)malloc(sizeof(assay_config_t));
    cfp->sections = DIMINUTO_TREE_EMPTY;
    cfp->cache = (assay_property_t *)0;

    return cfp;
}

assay_property_t * assay_config_cached(assay_config_t * cfp)
{
    return cfp->cache;
}

assay_config_t * assay_config_load(assay_config_t * cfp, FILE * fp)
{
    return 0;
}

void assay_config_destroy(assay_config_t * cfp)
{
    diminuto_tree_t * stp;
    assay_section_t * scp;
    diminuto_tree_t * ptp;
    assay_property_t * prp;

    for (stp = diminuto_tree_first(&(cfp->sections)); stp != DIMINUTO_TREE_NULL; stp = diminuto_tree_first(&(cfp->sections))) {
        scp = diminuto_containerof(assay_section_t, tree, stp);
        for (ptp = diminuto_tree_first(&(scp->properties)); ptp != DIMINUTO_TREE_NULL; ptp = diminuto_tree_first(&(scp->properties))) {
            prp = diminuto_containerof(assay_property_t, tree, ptp);
            diminuto_tree_remove(ptp);
            free((void *)(prp->key));
            free((void *)prp->value);
            free(prp);
        }
        diminuto_tree_remove(stp);
        free((void *)(scp->section));
        free(scp);
    }
    free(cfp);
}

/*******************************************************************************
 * SECTION PRIMITIVES
 ******************************************************************************/

static int section_compare(diminuto_tree_t * thisp, diminuto_tree_t * thatp)
{
    return strcmp(diminuto_containerof(assay_section_t, tree, thisp)->section, diminuto_containerof(assay_section_t, tree, thatp)->section);
}

static assay_section_t * section_find_close(assay_config_t * cfp, const char * section, int * rcp)
{
    diminuto_tree_t * stp;
    assay_section_t target;

    target.section = section;
    stp = diminuto_tree_search(cfp->sections, &(target.tree), section_compare, rcp);

    return (stp == DIMINUTO_TREE_NULL) ? (assay_section_t *)0 : diminuto_containerof(assay_section_t, tree, stp);
}

assay_section_t * assay_section_create(assay_config_t * cfp, const char * section)
{
    assay_section_t * scp;
    assay_section_t * tmp;
    int rc = 0;
    assay_action_t action;

    tmp = section_find_close(cfp, section, &rc);
    if (tmp == (assay_section_t *)0) {
        action = ROOT;
    } else if (rc < 0) {
        action = RIGHT;
    } else if (rc > 0) {
        action = LEFT;
    } else {
        action = NONE;
        scp = tmp;
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
            diminuto_tree_insert_left(&(scp->tree), &(tmp->tree));
            break;
        case RIGHT:
            diminuto_tree_insert_right(&(scp->tree), &(tmp->tree));
            break;
        default:
            break;
        }

    }

    return scp;
}

assay_section_t * assay_section_search(assay_config_t * cfp, const char * section)
{
    assay_section_t * scp = (assay_section_t *)0;
    int rc = 0;

    scp = section_find_close(cfp, section, &rc);
    if (rc != 0) {
        scp = (assay_section_t *)0;
    }

    return scp;
}

/*******************************************************************************
 * SECTION ITERATORS
 ******************************************************************************/

assay_section_t * assay_section_first(assay_config_t * cfp)
{
    assay_section_t * scp = (assay_section_t *)0;
    diminuto_tree_t * stp;

    if ((stp = diminuto_tree_first(&(cfp->sections))) != DIMINUTO_TREE_NULL) {
        scp = diminuto_containerof(assay_section_t, tree, stp);
    }

    return scp;
}

assay_section_t * assay_section_next(assay_section_t * scp)
{
    diminuto_tree_t * stp;

    if ((stp = diminuto_tree_next(&(scp->tree))) != DIMINUTO_TREE_NULL) {
        scp = diminuto_containerof(assay_section_t, tree, stp);
    } else {
        scp = (assay_section_t *)0;
    }

    return scp;
}

assay_section_t * assay_section_prev(assay_section_t * scp)
{
    diminuto_tree_t * stp;

    if ((stp = diminuto_tree_prev(&(scp->tree))) != DIMINUTO_TREE_NULL) {
        scp = diminuto_containerof(assay_section_t, tree, stp);
    } else {
        scp = (assay_section_t *)0;
    }

    return scp;
}

assay_section_t * assay_section_last(assay_config_t * cfp)
{
    assay_section_t * scp = (assay_section_t *)0;
    diminuto_tree_t * stp;

    if ((stp = diminuto_tree_last(&(cfp->sections))) != DIMINUTO_TREE_NULL) {
        scp = diminuto_containerof(assay_section_t, tree, stp);
    }

    return scp;
}

/*******************************************************************************
 * PROPERTY PRIMITIVES
 ******************************************************************************/

static int property_compare(diminuto_tree_t * thisp, diminuto_tree_t * thatp)
{
    return strcmp(diminuto_containerof(assay_property_t, tree, thisp)->key, diminuto_containerof(assay_property_t, tree, thatp)->key);
}

static assay_property_t * property_find_close(assay_section_t * scp, const char * key, int * rcp)
{
    diminuto_tree_t * ptp;
    assay_property_t target;

    target.key = key;
    ptp = diminuto_tree_search(scp->properties, &(target.tree), property_compare, rcp);

    return (ptp == DIMINUTO_TREE_NULL) ? (assay_property_t *)0 : diminuto_containerof(assay_property_t, tree, ptp);
}

assay_property_t * assay_property_create(assay_section_t * scp, const char * key)
{
    assay_property_t * prp;
    assay_property_t * tmp;
    int rc = 0;
    assay_action_t action;

    tmp = property_find_close(scp, key, &rc);
    if (tmp == (assay_property_t *)0) {
        action = ROOT;
    } else if (rc < 0) {
        action = RIGHT;
    } else if (rc > 0) {
        action = LEFT;
    } else {
        action = NONE;
        prp = tmp;
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
            diminuto_tree_insert_left(&(prp->tree), &(tmp->tree));
            break;
        case RIGHT:
            diminuto_tree_insert_right(&(prp->tree), &(tmp->tree));
            break;
        default:
            break;
        }

    }

    return prp;
}

assay_property_t * assay_property_search(assay_section_t * scp, const char * key)
{
	assay_property_t * prp = (assay_property_t *)0;
    int rc = 0;

     prp = property_find_close(scp, key, &rc);
     if (rc != 0) {
         prp = (assay_property_t *)0;
     }

    return prp;
}

void assay_property_destroy(assay_property_t * prp)
{
    if (prp->section->config->cache == prp) {
        prp->section->config->cache = (assay_property_t *)0;
    }
    diminuto_tree_remove(&(prp->tree));
    free((void *)(prp->key));
    free((void *)prp->value);
    free(prp);
}

/*******************************************************************************
 * PROPERTY ITERATORS
 ******************************************************************************/

assay_property_t * assay_property_first(assay_section_t * scp)
{
    assay_property_t * prp = (assay_property_t *)0;
    diminuto_tree_t * ptp;

    if ((ptp = diminuto_tree_first(&(scp->properties))) != DIMINUTO_TREE_NULL) {
        prp = diminuto_containerof(assay_property_t, tree, ptp);
    }

    return prp;
}

assay_property_t * assay_property_next(assay_property_t * prp)
{
    diminuto_tree_t * ptp;

    if ((ptp = diminuto_tree_next(&(prp->tree))) != DIMINUTO_TREE_NULL) {
        prp = diminuto_containerof(assay_property_t, tree, ptp);
    } else {
        prp = (assay_property_t *)0;
    }

    return prp;
}

assay_property_t * assay_property_prev(assay_property_t * prp)
{
    diminuto_tree_t * ptp;

    if ((ptp = diminuto_tree_prev(&(prp->tree))) != DIMINUTO_TREE_NULL) {
        prp = diminuto_containerof(assay_property_t, tree, ptp);
    } else {
        prp = (assay_property_t *)0;
    }

    return prp;
}

assay_property_t * assay_property_last(assay_section_t * scp)
{
    assay_property_t * prp = (assay_property_t *)0;
    diminuto_tree_t * ptp;

    if ((ptp = diminuto_tree_last(&(scp->properties))) != DIMINUTO_TREE_NULL) {
        prp = diminuto_containerof(assay_property_t, tree, ptp);
    }

    return prp;
}

/*******************************************************************************
 * GETTORS
 ******************************************************************************/

const char * assay_section_get(assay_section_t * scp)
{
    return scp->section;
}

const char * assay_key_get(assay_property_t * prp)
{
    return prp->key;
}

const void * assay_value_get(assay_property_t * prp, size_t * lengthp)
{
    const void * value;

    if ((value = prp->value) == (void *)0) {
        /* Do nothing. */
    } else if (lengthp == (size_t *)0) {
        /* Do nothing. */
    } else {
        *lengthp = prp->length;
    }

    return value;
}

/*******************************************************************************
 * SETTORS
 ******************************************************************************/

assay_property_t * assay_value_set(assay_property_t * prp, const void * value, size_t length)
{
     free((void *)prp->value);
     prp->length = length;
     prp->value = malloc(prp->length);
     memcpy((void *)prp->value, value, prp->length);

    return prp;
}

/*******************************************************************************
 * COMPOSITES
 ******************************************************************************/

const void * assay_config_get_binary(assay_config_t * cfp, const char * section, const char * key, size_t * lengthp)
{
    const char * value = (const char *)0;
    assay_property_t * prp;
    assay_section_t * scp;

    if ((scp = assay_section_search(cfp, section)) == (assay_section_t *)0) {
        /* Do nothing. */
    } else if ((prp = assay_property_search(scp, key)) == (assay_property_t *)0) {
        /* Do nothing. */
    } else {
        value = assay_value_get(prp, lengthp);
        cfp->cache = prp;
    }

    return value;
}

void assay_config_set_binary(assay_config_t * cfp, const char * section, const char * key, const void * value, size_t length)
{
    assay_property_t * prp = (assay_property_t *)0;
    assay_section_t * scp = (assay_section_t *)0;

    if (cfp->cache == (assay_property_t *)0) {
        /* Do nothing. */
    } else if (strcmp(cfp->cache->section->section, section) != 0) {
        /* Do nothing. */
    } else if (strcmp(cfp->cache->key, key) != 0) {
        /* Do nothing. */
    } else {
        prp = cfp->cache;
        scp = prp->section;
    }

    if (scp != (assay_section_t *)0) {
        /* Do nothing. */
    } else if ((scp = assay_section_search(cfp, section)) != (assay_section_t *)0) {
        /* Do nothing. */
    } else {
        scp = assay_section_create(cfp, section);
    }

    /* Invariant: scp points to the specified section. */

    if (prp != (assay_property_t *)0) {
        /* Do nothing. */
    } else if ((prp = assay_property_search(scp, key)) != (assay_property_t *)0) {
        cfp->cache = prp;
    } else {
        prp = assay_property_create(scp, key);
        cfp->cache = prp;
    }

    /* Invariant: prp points to the specified property in the section. */

    assay_value_set(prp, value, length);
}

/*******************************************************************************
 * AUDIT
 ******************************************************************************/

void * assay_config_audit(assay_config_t * cfp)
{
    void * result = (void *)0;
    diminuto_tree_t * stp;
    assay_section_t * scp;
    assay_section_t * sltp;
    assay_section_t * sgtp;
    diminuto_tree_t * ptp;
    assay_property_t * prp;
    assay_property_t * pltp;
    assay_property_t * pgtp;
    diminuto_tree_t * gttp;
    diminuto_tree_t * lttp;
    int line;

    if ((stp = diminuto_tree_audit(&(cfp->sections))) != (diminuto_tree_t *)0) {
        result = stp;
        line = __LINE__;
        goto exit;
    }
    for (stp = diminuto_tree_first(&(cfp->sections)); stp != DIMINUTO_TREE_NULL; stp = diminuto_tree_next(stp)) {
        scp = diminuto_containerof(assay_section_t, tree, stp);
        if (scp->config != cfp) {
            result = scp;
            line = __LINE__;
            goto exit;
        }
        if (scp->section == (const char *)0) {
            result = scp;
            line = __LINE__;
            goto exit;
        }
        lttp = diminuto_tree_prev(stp);
        gttp = diminuto_tree_next(stp);
        if ((lttp != DIMINUTO_TREE_NULL) && (gttp != DIMINUTO_TREE_NULL)) {
            sltp = diminuto_containerof(assay_section_t, tree, lttp);
            sgtp = diminuto_containerof(assay_section_t, tree, gttp);
            if (strcmp(sltp->section, sgtp->section) >= 0) {
                result = stp;
                line = __LINE__;
                goto exit;
            }
        }
        if ((ptp = diminuto_tree_audit(&(scp->properties))) != (diminuto_tree_t *)0) {
            result = ptp;
            line = __LINE__;
            goto exit;
        }
        for (ptp = diminuto_tree_first(&(scp->properties)); ptp != DIMINUTO_TREE_NULL; ptp = diminuto_tree_next(ptp)) {
            prp = diminuto_containerof(assay_property_t, tree, ptp);
            if (prp->section != scp) {
                result = prp;
                line = __LINE__;
                goto exit;
            }
            if (prp->key == (const char *)0) {
                result = prp;
                line = __LINE__;
                goto exit;
            }
            lttp = diminuto_tree_prev(ptp);
            gttp = diminuto_tree_next(ptp);
            if ((lttp != DIMINUTO_TREE_NULL) && (gttp != DIMINUTO_TREE_NULL)) {
                pltp = diminuto_containerof(assay_property_t, tree, lttp);
                pgtp = diminuto_containerof(assay_property_t, tree, gttp);
                if (strcmp(pltp->key, pgtp->key) >= 0) {
                    result = ptp;
                    line = __LINE__;
                    goto exit;
                }
            }
        }
    }

exit:

    if (result != (void *)0) {
        DIMINUTO_LOG_DEBUG("%s@%d: assay_config_audit FAILED!\n", __FILE__, line);
    }

    return result;
}
