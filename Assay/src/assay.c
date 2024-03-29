/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015-2023 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * This is the implementation of the Assay public API.
 */

#include <stdio.h>
#include <errno.h>
#include "assay.h"
#include "assay_parser.h"
#include "assay_fixup.h"
#include "assay_scanner.h"
#include "assay_scanner_annex.h"
#include "assay_parser_annex.h"
#include "com/diag/diminuto/diminuto_string.h"
#include "com/diag/diminuto/diminuto_containerof.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"
#include "com/diag/diminuto/diminuto_fd.h"
#include "com/diag/diminuto/diminuto_heap.h"

/*******************************************************************************
 * TYPES
 ******************************************************************************/

typedef enum AssayInsertion {
    NONE,
    ROOT,
    LEFT,
    RIGHT,
} assay_insertion_t;

/*******************************************************************************
 * GLOBALS
 ******************************************************************************/

size_t assay_buffer_default_size = ASSAY_BUFFER_DEFAULT_SIZE;

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/

const char ASSAY_CHARACTERS_SPECIAL[] = ASSAY_CHARACTERS_SPECIAL_STRING;

const char ASSAY_CHARACTERS_EXTRASPECIAL[] = ASSAY_CHARACTERS_EXTRASPECIAL_STRING;

const char ASSAY_SECTION_DEFAULT[] = ASSAY_SECTION_DEFAULT_STRING;

const char ASSAY_END_OF_TRANSMISSION[] = ASSAY_END_OF_TRANSMISSION_STRING;

/*******************************************************************************
 * CONFIGURATION PRIMITIVES
 ******************************************************************************/

assay_config_t * assay_config_create(void)
{
    assay_config_t * cfp;

    cfp = (assay_config_t *)diminuto_heap_malloc(sizeof(assay_config_t));
    memset(cfp, 0, sizeof(*cfp));
    cfp->file = "-";

    return cfp;
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
            diminuto_heap_free((void *)(prp->key));
            diminuto_heap_free((void *)prp->value);
            diminuto_heap_free(prp);
        }
        diminuto_tree_remove(stp);
        diminuto_heap_free((void *)(scp->name));
        diminuto_heap_free(scp);
    }
    diminuto_heap_free(cfp->vaction.buffer);
    diminuto_heap_free(cfp->kaction.buffer);
    diminuto_heap_free(cfp->saction.buffer);
    diminuto_heap_free(cfp->aaction.buffer);
    diminuto_heap_free(cfp->oaction.buffer);
    diminuto_heap_free(cfp);
}

int assay_config_error(assay_config_t * cfp)
{
    return ++(cfp->errors);
}

int assay_config_line(assay_config_t * cfp)
{
    return ++(cfp->lines);
}

/*******************************************************************************
 * CONFIGURATION GETTORS
 ******************************************************************************/

assay_section_t * assay_section_cached(assay_config_t * cfp)
{
    return cfp->section;
}

assay_property_t * assay_property_cached(assay_config_t * cfp)
{
    return cfp->property;
}

int assay_config_errors(assay_config_t * cfp)
{
    return cfp->errors;
}

/*******************************************************************************
 * SECTION PRIMITIVES
 ******************************************************************************/

static int section_compare(const diminuto_tree_t * thisp, const diminuto_tree_t * thatp)
{
    return strcmp(diminuto_containerof(const assay_section_t, tree, thisp)->name, diminuto_containerof(const assay_section_t, tree, thatp)->name);
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
    assay_insertion_t insertion;

    tmp = section_find_close(cfp, name, &rc);
    if (tmp == (assay_section_t *)0) {
        insertion = ROOT;
    } else if (rc < 0) {
        insertion = RIGHT;
    } else if (rc > 0) {
        insertion = LEFT;
    } else {
        insertion = NONE;
        scp = tmp;
    }

    if (insertion != NONE) {

        scp = (assay_section_t *)diminuto_heap_malloc(sizeof(assay_section_t));
        diminuto_tree_init(&(scp->tree));
        scp->config = cfp;
        scp->name = diminuto_string_strdup(name);
        scp->properties = DIMINUTO_TREE_EMPTY;

        switch (insertion) {
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

static int property_compare(const diminuto_tree_t * thisp, const diminuto_tree_t * thatp)
{
    return strcmp(diminuto_containerof(const assay_property_t, tree, thisp)->key, diminuto_containerof(const assay_property_t, tree, thatp)->key);
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
    assay_insertion_t insertion;

    tmp = property_find_close(scp, key, &rc);
    if (tmp == (assay_property_t *)0) {
        insertion = ROOT;
    } else if (rc < 0) {
        insertion = RIGHT;
    } else if (rc > 0) {
        insertion = LEFT;
    } else {
        insertion = NONE;
        prp = tmp;
    }

    if (insertion != NONE) {

        prp = (assay_property_t *)diminuto_heap_malloc(sizeof(assay_property_t));
        diminuto_tree_init(&(prp->tree));
        prp->section = scp;
        prp->key = diminuto_string_strdup(key);
        prp->value = 0;
        prp->length = 0;

        switch (insertion) {
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
    if (prp->section->config->property == prp) {
        prp->section->config->property = (assay_property_t *)0;
    }
    diminuto_tree_remove(&(prp->tree));
    diminuto_heap_free((void *)(prp->key));
    diminuto_heap_free((void *)(prp->value));
    diminuto_heap_free(prp);
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

void * assay_property_value_get(assay_property_t * prp, size_t * lengthp)
{
    void * value;

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
    diminuto_heap_free((void *)prp->value);
    prp->length = length;
    prp->value = diminuto_heap_malloc(prp->length);
    memcpy((void *)prp->value, value, prp->length);

    return prp;
}

/*******************************************************************************
 * COMPOSITE OPERATORS
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

void * assay_config_read_binary(assay_config_t * cfp, const char * name, const char * key, size_t * lengthp)
{
    char * value = (char *)0;
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

void assay_config_destroy_binary(assay_config_t * cfp, const char * name, const char * key)
{
    assay_section_t * scp;
    assay_property_t * prp;

    if ((scp = section_resolve(cfp, name, 0)) == (assay_section_t *)0) {
	/* Do nothing. */
    } else if ((prp = property_resolve(scp, key, 0)) == (assay_property_t *)0) {
	/* Do nothing. */
    } else {
	assay_property_destroy(prp);
    }
}

/*******************************************************************************
 * IMPORTERS
 ******************************************************************************/

assay_config_t * assay_config_import_stream(assay_config_t * cfp, FILE * stream)
{
    FILE * priorstream;
    assay_scanner_lexical_t lxp;
    int rc;

    priorstream = cfp->stream;
    cfp->stream = stream;

    lxp = assay_scanner_create(cfp, stream);

    /*
     * We only have to call parse() once. The Assay grammar is written to
     * consume the entire input stream, and the Assay scanner is written to
     * terminate the input stream either at EOF or at the ^D character. This
     * allows us to either scan entire files (in the former case) or the next
     * message on a socket (in the latter case). Other grammars may require
     * that the application iterate calling its parse() function.
     */

    rc = assay_parser_parse(lxp);

    if (priorstream == (FILE *)0) {
        assay_parser_fini(lxp);
    }

    assay_scanner_destroy(lxp);

    cfp->stream = priorstream;

    if (rc != 0) {
        DIMINUTO_LOG_ERROR("assay_config_import_stream: *ERROR* config=%p stream=%p rc=%d\n", cfp, stream, rc);
        cfp = (assay_config_t *)0;
    }

    return cfp;
}

assay_config_t * assay_config_import_stream_close(assay_config_t * cfp, FILE * stream)
{
    cfp = assay_config_import_stream(cfp, stream);
    (void)fclose(stream);
    return cfp;
}

assay_config_t * assay_config_import_file(assay_config_t * cfp, const char * file)
{
    assay_config_t * result;
    const char * priorfile;
    int priorlines;
    FILE * fp;

    priorfile = cfp->file;
    priorlines = cfp->lines;

    cfp->file = file;
    cfp->lines = 0;

    DIMINUTO_LOG_INFORMATION("assay_config_import_file: include config=%p file=\"%s\" line=%d include=\"%s\"\n", cfp, priorfile, priorlines, file);

    if ((file[0] == '-') && (file[1] == '\0')) {
        result = assay_config_import_stream(cfp, stdin);
    } else if ((fp = fopen(file, "re")) != (FILE *)0) {
        result = assay_config_import_stream_close(cfp, fp);
    } else {
        int errors;
        errors = assay_config_error(cfp);
        DIMINUTO_LOG_WARNING("assay_config_import_file: *%s* config=%p file=\"%s\" line=%d include=\"%s\" errors=%d\n", strerror(errno), cfp, priorfile, priorlines, file, errors);
        result = (assay_config_t *)0;
    }

    cfp->lines = priorlines;
    cfp->file = priorfile;

    return result;
}

assay_config_t * assay_config_import_command(assay_config_t * cfp, const char * command)
{
    assay_config_t * result;
    const char * priorfile;
    int priorlines;
    FILE * fp;

    priorfile = cfp->file;
    priorlines = cfp->lines;

    cfp->file = command;
    cfp->lines = 0;

    DIMINUTO_LOG_INFORMATION("assay_config_import_command: exec config=%p file=\"%s\" line=%d command=\"%s\"\n", cfp, priorfile, priorlines, command);

    if ((fp = popen(command, "re")) != (FILE *)0) {
        result = assay_config_import_stream(cfp, fp);
        pclose(fp);
    } else {
        int errors;
        errors = assay_config_error(cfp);
        DIMINUTO_LOG_WARNING("assay_config_import_command: *%s* config=%p file=\"%s\" line=%d command=\"%s\" errors=%d\n", strerror(errno), cfp, priorfile, priorlines, command, errors);
        result = (assay_config_t *)0;
    }

    cfp->lines = priorlines;
    cfp->file = priorfile;

    return result;
}

/*******************************************************************************
 * EXPORTERS
 ******************************************************************************/

static int config_export_stream_isinteractive(FILE * stream)
{
    diminuto_fd_type_t type;
    int interactive;

    /*
     * Here we try to determine what kind of file stream we have, batch
     * (like a regular file) or interactive (like a socket, pipe, or
     * character device). For sockets, character devices that are not TTYs,
     * or FIFOs, we want to append the EOT sequences to the end of the
     * output sequence to signal the lexical scanner on the other end that
     * the input sequence has ended. For the most part, we don't want to do
     * this for TTYs since generally that's someone writing to a display.
     * If you are really old school and are actually writing to a serial port
     * to a computer on the receiving side, you probably want to use the special
     * export send function to force the EOT to be added, or modify this source
     * code to put the TTY case in the interactive part. Note that one of the
     * side effects of this is that if you export to stdout, but then pipe it
     * to, say, the tee(1) command, it will appear to be interactive, because
     * from the POV of export, it is. N.B. the classifications of the output
     * stream below deliberately may not match those for classifying the input
     * stream.
     */

    type = diminuto_fd_type(fileno(stream));
    switch (type) {
    case DIMINUTO_FS_TYPE_SOCKET:
    case DIMINUTO_FS_TYPE_CHARACTERDEV:
    case DIMINUTO_FS_TYPE_FIFO:
        interactive = !0;
        break;
    case DIMINUTO_FS_TYPE_UNKNOWN:
    case DIMINUTO_FS_TYPE_TTY:
    case DIMINUTO_FS_TYPE_SYMLINK:
    case DIMINUTO_FS_TYPE_FILE:
    case DIMINUTO_FS_TYPE_BLOCKDEV:
    case DIMINUTO_FS_TYPE_DIRECTORY:
        interactive = 0;
        break;
    }

    return interactive;
}

/*
 * This isn't part of the public API but is exposed never the less just in case.
 */
assay_config_t * assay_config_export_stream_generic(assay_config_t * cfp, FILE * stream, int interactive)
{
    char * buffer;
    char * here;
    assay_section_t * scp;
    assay_property_t * prp;
    const char * name;
    const char * key;
    const char * value;
    size_t size;
    size_t fsize;
    size_t tsize;

    buffer = (char *)diminuto_heap_malloc(size = assay_buffer_default_size * 4 /* '\xFF' */);
    for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
        name = assay_section_name_get(scp);
        fsize = strlen(name);
        tsize = (fsize * 4 /* '\xFF' */) + 1 /* '\0' */;
        if (tsize > size) {
            diminuto_heap_free(buffer);
            buffer = (char *)diminuto_heap_malloc(size = tsize);
        }
        diminuto_escape_expand(buffer, name, tsize, fsize, ASSAY_CHARACTERS_SPECIAL);
        fprintf(stream, "[%s]\n", buffer);
        for (prp = assay_property_first(scp); prp != (assay_property_t *)0; prp = assay_property_next(prp)) {
            key = assay_property_key_get(prp);
            fsize = strlen(key);
            tsize = (fsize * 4 /* '\xFF' */) + 1 /* '\0' */;
            if (tsize > size) {
                diminuto_heap_free(buffer);
                buffer = (char *)diminuto_heap_malloc(size = tsize);
            }
            diminuto_escape_expand(buffer, key, tsize, fsize, ASSAY_CHARACTERS_SPECIAL);
            fprintf(stream, "%s=", buffer);
            value = (const char *)assay_property_value_get(prp, &fsize);
            fsize -= 1 /* '\0' */;
            tsize = (fsize * 4 /* '\xFF' */) + 1 /* '\0' */;
            if (tsize > size) {
                diminuto_heap_free(buffer);
                buffer = (char *)diminuto_heap_malloc(size = tsize);
            }
            /*
             * If the first character of the value is a space, it is a special
             * case and must be escaped. We could escape all the spaces, but we
             * prefer not to just for readability of the resulting stream.
             */
            here = buffer;
            if (value[0] == ' ') {
                *(here++) = '\\';
                --tsize;
            }
            diminuto_escape_expand(here, value, tsize, fsize, ASSAY_CHARACTERS_EXTRASPECIAL);
            fprintf(stream, "%s\n", buffer);
        }
    }

    if (buffer != (char *)0) {
        diminuto_heap_free(buffer);
    }

    if (interactive) {
        (void)fputs(ASSAY_END_OF_TRANSMISSION, stream);
    }

    return cfp;
}

assay_config_t * assay_config_export_stream(assay_config_t * cfp, FILE * stream)
{
    cfp = assay_config_export_stream_generic(cfp, stream, config_export_stream_isinteractive(stream));
    (void)fflush(stream);

    return cfp;
}

assay_config_t * assay_config_export_stream_close(assay_config_t * cfp, FILE * stream)
{
    cfp = assay_config_export_stream_generic(cfp, stream, config_export_stream_isinteractive(stream));
    (void)fclose(stream);

    return cfp;
}

assay_config_t * assay_config_export_stream_send(assay_config_t * cfp, FILE * stream)
{
    cfp = assay_config_export_stream_generic(cfp, stream, !0);
    (void)fflush(stream);

    return cfp;
}

assay_config_t * assay_config_export_file(assay_config_t * cfp, const char * file)
{
    FILE * fp;

    if ((file[0] == '-') && (file[1] == '\0')) {
        cfp = assay_config_export_stream(cfp, stdout);
    } else if ((fp = fopen(file, "w")) != (FILE *)0) {
        cfp = assay_config_export_stream_close(cfp, fp);
    } else {
        cfp = (assay_config_t *)0;
    }

    return cfp;
}


/*******************************************************************************
 * AUDITORS
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
        } else if (diminuto_escape_printable(value)) {
            DIMINUTO_LOG_DEBUG("assay_property_t@%p[%zu]: key=\"%s\" value=\"%s\"[%zu]\n", prp, sizeof(*prp), key, value, length);
        } else {
            DIMINUTO_LOG_DEBUG("assay_property_t@%p[%zu]: key=\"%s\" value=<\n", prp, sizeof(*prp), key);
            diminuto_dump(diminuto_log_stream(), value, length);
            DIMINUTO_LOG_DEBUG(">[%zu]\n", length);
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
        DIMINUTO_LOG_DEBUG("assay_section_t@%p[%zu]: name=\"%s\"\n", scp, sizeof(*scp), assay_section_name_get(scp));
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
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: section=%p\n", cfp, cfp->section);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: property=%p\n", cfp, cfp->property);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: stream=%p\n", cfp, cfp->stream);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: file=\"%s\"\n", cfp, cfp->file);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: lines=%d\n", cfp, cfp->lines);
        DIMINUTO_LOG_DEBUG("assay_config_t@%p: errors=%d\n", cfp, cfp->errors);
        for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
            assay_section_log(scp);
        }
    }
}

void * assay_config_audit(assay_config_t * cfp)
{
    void * result = (void *)0;
    assay_section_t * scp;
    assay_section_t * sltp;
    assay_section_t * sgtp;
    assay_property_t * prp;
    assay_property_t * pltp;
    assay_property_t * pgtp;
    diminuto_tree_t * stp;
    diminuto_tree_t * ptp;
    const char * type = "";
    int line;

    type = "array_config_t";
    result = cfp;
    if (cfp == (assay_config_t *)0) {
        line = __LINE__;
        goto exit;
    }
    if (cfp->section == (assay_section_t *)0) {
        /* Do nothing. */
    } else if (cfp->section->config == cfp) {
        /* Do nothing. */
    } else {
        line = __LINE__;
        goto exit;
    }
    if (cfp->property == (assay_property_t *)0) {
        /* Do nothing. */
    } else if (cfp->property->section == cfp->section) {
        /* Do nothing. */
    } else {
        line = __LINE__;
        goto exit;
    }
    type = "array_section_t";
    if ((stp = diminuto_tree_audit(&(cfp->sections))) != (diminuto_tree_t *)0) {
        result = diminuto_containerof(assay_section_t, tree, stp);
        line = __LINE__;
        goto exit;
    }
    for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
        result = scp;
        if (scp->config != cfp) {
            line = __LINE__;
            goto exit;
        }
        if (scp->name == (const char *)0) {
            line = __LINE__;
            goto exit;
        }
        if ((sltp = assay_section_prev(scp)) == (assay_section_t *)0) {
            /* Do nothing. */
        } else if ((sgtp = assay_section_next(scp)) == (assay_section_t *)0) {
            /* Do nothing. */
        } else if (strcmp(sltp->name, sgtp->name) < 0) {
            /* Do nothing. */
        } else {
            line = __LINE__;
            goto exit;
        }
        type = "array_property_t";
        if ((ptp = diminuto_tree_audit(&(scp->properties))) != (diminuto_tree_t *)0) {
            result = diminuto_containerof(assay_property_t, tree, ptp);
            line = __LINE__;
            goto exit;
        }
        for (prp = assay_property_first(scp); prp != (assay_property_t *)0; prp = assay_property_next(prp)) {
            result = prp;
            if (prp->section != scp) {
                line = __LINE__;
                goto exit;
            }
            if (prp->key == (const char *)0) {
                line = __LINE__;
                goto exit;
            }
            if ((pltp = assay_property_prev(prp)) == (assay_property_t *)0) {
                /* Do nothing. */
            } else if ((pgtp = assay_property_next(prp)) == (assay_property_t *)0) {
                /* Do nothing. */
            } else if (strcmp(pltp->key, pgtp->key) < 0) {
                /* Do nothing. */
            } else {
                line = __LINE__;
                goto exit;
            }
        }
    }
    result = (void *)0;

exit:

    if (result != (void *)0) {
        DIMINUTO_LOG_WARNING("assay_config_audit: *audit failed* config=%p object=%p type=%s check=%d\n", cfp, result, type, line);
    }

    return result;
}
