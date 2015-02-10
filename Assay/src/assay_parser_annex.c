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
#include "com/diag/assay/assay_parser.h"

typedef struct AssayParserAction {
    const char * name;
    char * buffer;
    size_t length;
    size_t index;
} assay_parser_action_t;

#define ASSAY_PARSER_ACTION_INIT(_NAME_) { _NAME_, 0, 0, 0 }

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
    fprintf(stderr, "%s[%zu]=\"%s\"[%zu]\n", ap->name, ap->length, ap->buffer, ap->index);
}

/******************************************************************************/

static assay_parser_action_t section = ASSAY_PARSER_ACTION_INIT("section");

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
}

/******************************************************************************/

static assay_parser_action_t keyword = ASSAY_PARSER_ACTION_INIT("keyword");

void assay_parser_keyword_begin(int ch)
{
    assay_parser_action_begin(&keyword, ch);
}

void assay_parser_keyword_next(int ch)
{
    assay_parser_action_next(&keyword, ch);
}

void assay_parser_keyword_end(void)
{
    assay_parser_action_end(&keyword);
}

/******************************************************************************/

static assay_parser_action_t value = ASSAY_PARSER_ACTION_INIT("value");

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
}

/******************************************************************************/
