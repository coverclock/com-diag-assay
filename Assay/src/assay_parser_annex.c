/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include "assay_parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "com/diag/assay/assay_parser.h"
#include "com/diag/diminuto/diminuto_dump.h"

typedef struct AssayParserAction {
    char * buffer;
    size_t length;
    size_t index;
} assay_parser_action_t;

/******************************************************************************/

static int debug = !0;

int assay_parser_debug(int enable)
{
    int was;
    was = debug;
    debug = enable;
    return was;
}

/******************************************************************************/

static void assay_parser_action_next(assay_parser_action_t * ap, int ch)
{
    if (ap->index >= ap->length) {
        char * old;
        old = ap->buffer;
        ap->length *= 2;
        ap->buffer = malloc(ap->length);
        memcpy(ap->buffer, old, ap->index);
        free(old);
    }
    ap->buffer[ap->index++] = ch;
}

static void assay_parser_action_begin(assay_parser_action_t * ap, int ch)
{
    if (ap->buffer == (char *)0) {
        ap->length = 64;
        ap->buffer = malloc(ap->length);
    }
    ap->index = 0;
    assay_parser_action_next(ap, ch);
}

static void assay_parser_action_end(assay_parser_action_t * ap)
{
    assay_parser_action_next(ap, '\0');
}

/******************************************************************************/

static assay_parser_action_t section = { 0 };

void assay_parser_section_begin(int ch)
{
    assay_parser_action_begin(&section, ch);
}

void assay_parser_section_next(int ch)
{
    assay_parser_action_next(&section, ch);
}

void assay_parser_section_end(void)
{
    assay_parser_action_end(&section);
    if (debug) {
        fprintf(stderr, "assay_parser: section[%zu]=\"%s\"[%zu]\n", section.length, section.buffer, section.index);
    }
}

/******************************************************************************/

static assay_parser_action_t key = { 0 };

void assay_parser_key_begin(int ch)
{
    assay_parser_action_begin(&key, ch);
}

void assay_parser_key_next(int ch)
{
    assay_parser_action_next(&key, ch);
}

void assay_parser_key_end(void)
{
    assay_parser_action_end(&key);
    if (debug) {
        fprintf(stderr, "assay_parser: key[%zu]=\"%s\"[%zu]\n", key.length, key.buffer, key.index);
    }
}

/******************************************************************************/

static assay_parser_action_t value = { 0 };

void assay_parser_value_begin(int ch)
{
   assay_parser_action_begin(&value, ch);
}

void assay_parser_value_next(int ch)
{
    assay_parser_action_next(&value, ch);
}

void assay_parser_value_end(void)
{
    assay_parser_action_end(&value);
    if (debug) {
        int printable = !0;
        size_t ii;
        size_t limit;
        limit = value.index - 1; /* Ignore trailing NUL. */
        for (ii = 0; ii < limit; ++ii) {
            if (!isprint(value.buffer[ii])) {
                printable = 0;
                break;
            }
        }
        if (printable) {
            fprintf(stderr, "assay_parser: value[%zu]=\"%s\"[%zu]\n", value.length, value.buffer, value.index);
        } else {
            fprintf(stderr, "assay_parser: value[%zu]=<\n", value.length);
            diminuto_dump(stderr, value.buffer, value.index);
            fprintf(stderr, ">[%zu]\n", value.index);
        }
    }
}

/******************************************************************************/
