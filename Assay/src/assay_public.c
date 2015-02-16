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
#include "com/diag/diminuto/diminuto_dump.h"

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
    const char *        name;
    diminuto_tree_t *   properties;
};

struct AssayConfig {
    diminuto_tree_t *   sections;
    assay_section_t *   section;
    assay_property_t *  property;
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
    cfp->section = (assay_section_t *)0;
    cfp->property = (assay_property_t *)0;

    return cfp;
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
        free((void *)(scp->name));
        free(scp);
    }
    free(cfp);
}

/*******************************************************************************
 * SECTION PRIMITIVES
 ******************************************************************************/

static int section_compare(diminuto_tree_t * thisp, diminuto_tree_t * thatp)
{
    return strcmp(diminuto_containerof(assay_section_t, tree, thisp)->name, diminuto_containerof(assay_section_t, tree, thatp)->name);
}

static assay_section_t * section_find_close(assay_config_t * cfp, const char * name, int * rcp)
{
    diminuto_tree_t * stp;
    assay_section_t target;

    target.name = name;
    stp = diminuto_tree_search(cfp->sections, &(target.tree), section_compare, rcp);

    return (stp == DIMINUTO_TREE_NULL) ? (assay_section_t *)0 : diminuto_containerof(assay_section_t, tree, stp);
}

assay_section_t * assay_section_create(assay_config_t * cfp, const char * name)
{
    assay_section_t * scp;
    assay_section_t * tmp;
    int rc = 0;
    assay_action_t action;

    tmp = section_find_close(cfp, name, &rc);
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
        scp->name = strdup(name);
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

assay_section_t * assay_section_search(assay_config_t * cfp, const char * name)
{
    assay_section_t * scp = (assay_section_t *)0;
    int rc = 0;

    scp = section_find_close(cfp, name, &rc);
    if (rc != 0) {
        scp = (assay_section_t *)0;
    }

    return scp;
}

assay_section_t * assay_section_cached(assay_config_t * cfp)
{
    return cfp->section;
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
 * SECTION GETTORS
 ******************************************************************************/

const char * assay_section_name_get(assay_section_t * scp)
{
    return scp->name;
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

assay_property_t * assay_property_cached(assay_config_t * cfp)
{
    return cfp->property;
}

void assay_property_destroy(assay_property_t * prp)
{
    if (prp->section->config->property == prp) {
        prp->section->config->property = (assay_property_t *)0;
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
 * PROPERTY GETTORS
 ******************************************************************************/

const char * assay_property_key_get(assay_property_t * prp)
{
    return prp->key;
}

const void * assay_property_value_get(assay_property_t * prp, size_t * lengthp)
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
 * PROPERTY SETTORS
 ******************************************************************************/

assay_property_t * assay_property_value_set(assay_property_t * prp, const void * value, size_t length)
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

static inline void section_cache(assay_config_t * cfp, assay_section_t * scp)
{
    cfp->section = scp;
    cfp->property = (assay_property_t *)0;
}

static assay_section_t * section_resolve(assay_config_t * cfp, const char * name, int create)
{
    assay_section_t * scp = (assay_section_t *)0;

    if (cfp->section == (assay_section_t *)0) {
        /* Do nothing. */
    } else if (strcmp(cfp->section->name, name) != 0) {
        /* Do nothing. */
    } else {
        scp = cfp->section;
    }

    if (scp != (assay_section_t *)0) {
        /* Do nothing. */
    } else if ((scp = assay_section_search(cfp, name)) == (assay_section_t *)0) {
        /* Do nothing. */
    } else {
        section_cache(cfp, scp);
    }

    if (scp != (assay_section_t *)0) {
        /* Do nothing. */
    } else if (!create) {
        /* Do nothing. */
    } else if ((scp = assay_section_create(cfp, name)) == (assay_section_t *)0) {
        /* Do nothing. */
    } else {
        section_cache(cfp, scp);
    }

    return scp;
}

static inline void property_cache(assay_section_t * scp, assay_property_t * prp)
{
    scp->config->property = prp;
    scp->config->section = prp->section;
}

static assay_property_t * property_resolve(assay_section_t * scp, const char * key, int create)
{
    assay_property_t * prp = (assay_property_t *)0;

    if (scp->config->property == (assay_property_t *)0) {
        /* Do nothing. */
    } else if (scp->config->section != scp) {
        /* Do nothing. */
    } else if (strcmp(scp->config->property->key, key) != 0) {
        /* Do nothing. */
    } else {
        prp = scp->config->property;
    }

    if (prp != (assay_property_t *)0) {
        /* Do nothing. */
    } else if ((prp = assay_property_search(scp, key)) == (assay_property_t *)0) {
        /* Do nothing. */
    } else {
        property_cache(scp, prp);
    }

    if (prp != (assay_property_t *)0) {
        /* Do nothing. */
    } else if (!create) {
        /* Do nothing. */
    } else if ((prp = assay_property_create(scp, key)) == (assay_property_t *)0) {
        /* Do nothing. */
    } else {
        property_cache(scp, prp);
    }

    return prp;
}

const void * assay_config_read_binary(assay_config_t * cfp, const char * name, const char * key, size_t * lengthp)
{
    const char * value = (const char *)0;
    assay_section_t * scp;
    assay_property_t * prp;

    if ((scp = section_resolve(cfp, name, 0)) == (assay_section_t *)0) {
        /* Do nothing. */
    } else if ((prp = property_resolve(scp, key, 0)) == (assay_property_t *)0) {
        /* Do nothing. */
    } else {
        value = assay_property_value_get(prp, lengthp);
    }

    return value;
}

void assay_config_write_binary(assay_config_t * cfp, const char * name, const char * key, const void * value, size_t length)
{
    assay_property_value_set(property_resolve(section_resolve(cfp, name, !0), key, !0), value, length);
}

/*******************************************************************************
 * AUDIT
 ******************************************************************************/

void assay_property_log(assay_property_t * prp)
{
    if (!DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
        /* Do nothing. */
    } else if (prp == (assay_property_t *)0) {
        DIMINUTO_LOG_DEBUG("assay_property_t@%p[%zu]\n", prp, sizeof(*prp));
    } else {
        const char * key;
        const char * value;
        size_t length;
        key = assay_property_key_get(prp);
        value = assay_property_value_get(prp, &length);
        if (value == (void *)0) {
            DIMINUTO_LOG_DEBUG("assay_property_t@%p[%zu]: key=\"%s\" value==%p\n", prp, sizeof(*prp), key, value);
        } else {
            size_t ii;
            int printable;
            printable = !0;
            for (ii = 0; ii < length - 1; ++ii) {
                if (!isprint(value[ii])) {
                    printable = 0;
                    break;
                }
            }
            if (printable) {
                DIMINUTO_LOG_DEBUG("assay_property_t@%p[%zu]: key=\"%s\" value=\"%s\"[%zu]\n", prp, sizeof(*prp), key, value, length);
            } else {
                DIMINUTO_LOG_DEBUG("assay_property_t@%p[%zu]: key=\"%s\" value=<\n", prp, sizeof(*prp), key);
                diminuto_dump(diminuto_log_stream(), value, length);
                DIMINUTO_LOG_DEBUG(">[%zu]\n", length);
            }
        }
    }
}

void assay_section_log(assay_section_t * scp)
{
    if (!DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
        /* Do nothing. */
    } else if (scp == (assay_section_t *)0) {
        DIMINUTO_LOG_DEBUG("assay_section_t@%p[%zu]\n", scp, sizeof(*scp));
    } else {
        assay_property_t * prp;
        DIMINUTO_LOG_DEBUG("assay_section_t@%p[%zu]: [%s]\n", scp, sizeof(*scp), assay_section_name_get(scp));
        for (prp = assay_property_first(scp); prp != (assay_property_t *)0; prp = assay_property_next(prp)) {
        	assay_property_log(prp);
        }
    }
}

void assay_config_log(assay_config_t * cfp)
{
    if (!DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
        /* Do nothing. */
    } else if (cfp == (assay_config_t *)0) {
        DIMINUTO_LOG_DEBUG("assay_config_t@%p[%zu]\n", cfp, sizeof(*cfp));
    } else {
        assay_section_t * scp;
        DIMINUTO_LOG_DEBUG("assay_config_t@%p[%zu]:\n", cfp, sizeof(*cfp));
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: section:\n", cfp);
        assay_section_log(cfp->section);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: property:\n", cfp);
        assay_property_log(cfp->property);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: sections:\n", cfp);
        for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
        	assay_section_log(scp);
        }
    }
}

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
        if (scp->name == (const char *)0) {
            result = scp;
            line = __LINE__;
            goto exit;
        }
        lttp = diminuto_tree_prev(stp);
        gttp = diminuto_tree_next(stp);
        if ((lttp != DIMINUTO_TREE_NULL) && (gttp != DIMINUTO_TREE_NULL)) {
            sltp = diminuto_containerof(assay_section_t, tree, lttp);
            sgtp = diminuto_containerof(assay_section_t, tree, gttp);
            if (strcmp(sltp->name, sgtp->name) >= 0) {
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
