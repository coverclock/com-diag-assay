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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "com/diag/assay/assay.h"
#include "com/diag/assay/assay_parser.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_log.h"

/*******************************************************************************
 * ACTION
 ******************************************************************************/

typedef struct AssayParserAction {
    char * buffer;
    size_t length;
    size_t index;
} assay_parser_action_t;

static void assay_parser_action_begin(assay_parser_action_t * ap)
{
    if (ap->buffer == (char *)0) {
        ap->length = 64;
        ap->buffer = malloc(ap->length);
    }
    ap->index = 0;
}

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

static void assay_parser_action_end(assay_parser_action_t * ap)
{
    assay_parser_action_next(ap, '\0');
}

/*******************************************************************************
 * VALUE
 ******************************************************************************/

static assay_parser_action_t value = { 0 };

void assay_parser_value_begin(void)
{
   assay_parser_action_begin(&value);
}

void assay_parser_value_next(int ch)
{
    assay_parser_action_next(&value, ch);
}

void assay_parser_value_end(void)
{
    assay_parser_action_end(&value);
    if (DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_INFORMATION)) {
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
            DIMINUTO_LOG_INFORMATION("assay_parser: value[%zu]=\"%s\"[%zu]\n", value.length, value.buffer, value.index);
        } else {
            DIMINUTO_LOG_INFORMATION("assay_parser: value[%zu]:\n", value.length);
            diminuto_dump(diminuto_log_stream(), value.buffer, value.index);
        }
    }
}

/*******************************************************************************
 * KEY
 ******************************************************************************/

static assay_parser_action_t key = { 0 };

void assay_parser_key_begin(void)
{
    assay_parser_action_begin(&key);
    assay_parser_action_begin(&value);
    assay_parser_action_end(&value);
}

void assay_parser_key_next(int ch)
{
    assay_parser_action_next(&key, ch);
}

void assay_parser_key_end(void)
{
    assay_parser_action_end(&key);
    DIMINUTO_LOG_INFORMATION("assay_parser: key[%zu]=\"%s\"[%zu]\n", key.length, key.buffer, key.index);
}

/*******************************************************************************
 * SECTION
 ******************************************************************************/

static assay_parser_action_t section = { 0 };

void assay_parser_section_begin(void)
{
    assay_parser_action_begin(&section);
}

void assay_parser_section_next(int ch)
{
    assay_parser_action_next(&section, ch);
}

void assay_parser_section_end(void)
{
    assay_parser_action_end(&section);
    DIMINUTO_LOG_INFORMATION("assay_parser: section[%zu]=\"%s\"[%zu]\n", section.length, section.buffer, section.index);
}

/*******************************************************************************
 * PROPERTY
 ******************************************************************************/

assay_config_t * assay_parser_config = (assay_config_t *)0;

void assay_parser_property_assign(void)
{
    if (assay_parser_config != (assay_config_t *)0) {
        assay_config_write_binary(assay_parser_config, section.buffer, key.buffer, value.buffer, value.length);
    }
}
