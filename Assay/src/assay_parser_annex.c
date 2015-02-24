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
#include "com/diag/assay/assay_scanner.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_escape.h"

/*******************************************************************************
 * TYPES
 ******************************************************************************/

typedef struct AssayParserAction {
    char * buffer;
    size_t length;
    size_t index;
} assay_parser_action_t;

/*******************************************************************************
 * STATE
 ******************************************************************************/

static int debug = 0;
static assay_config_t * config = (assay_config_t *)0;
static const char * file = "-";
static int line = 0;

static assay_parser_action_t operator = { 0 };
static assay_parser_action_t argument = { 0 };
static assay_parser_action_t section = { 0 };
static assay_parser_action_t key = { 0 };
static assay_parser_action_t value = { 0 };

/*******************************************************************************
 * ACTION
 ******************************************************************************/

static void action_begin(assay_parser_action_t * ap)
{
    if (ap->buffer == (char *)0) {
        ap->length = 8;
        ap->buffer = malloc(ap->length);
    }
    ap->index = 0;
}

static void action_next(assay_parser_action_t * ap, int ch)
{
    if (ap->buffer == (char *)0) {
        action_begin(ap);
    }
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

static void action_end(assay_parser_action_t * ap)
{
    if ((ap->index <= 0) || (ap->buffer[ap->index - 1] != '\0')) {
        action_next(ap, '\0');
    }
}

/*******************************************************************************
 * OPERATOR
 ******************************************************************************/

void assay_parser_operator_begin(void)
{
    action_begin(&operator);
}

void assay_parser_operator_next(int ch)
{
    action_next(&operator, ch);
}

void assay_parser_operator_end(void)
{
    action_end(&operator);
    if (debug) {
        DIMINUTO_LOG_DEBUG("assay_parser: operator[%zu]=\"%s\"[%zu]\n", operator.length, operator.buffer, operator.index);
    }
}

/*******************************************************************************
 * ARGUMENT
 ******************************************************************************/

void assay_parser_argument_begin(void)
{
    action_begin(&argument);
}

void assay_parser_argument_next(int ch)
{
    action_next(&argument, ch);
}

void assay_parser_argument_end(void)
{
    action_end(&argument);
    if (debug) {
        DIMINUTO_LOG_DEBUG("assay_parser: argument[%zu]=\"%s\"[%zu]\n", argument.length, argument.buffer, argument.index);
    }
}

/*******************************************************************************
 * OPERATION
 ******************************************************************************/

void assay_parser_operation_execute(void)
{
    if (config == (assay_config_t *)0) {
        /* Do nothing. */
    } else if (strcmp(operator.buffer, "include") == 0) {
        assay_config_load_file(config, argument.buffer);
    } else {
        assay_config_error(config);
        DIMINUTO_LOG_WARNING("assay_parser_operation_execute: *invalid* config=%p operator=\"%s\" argument=\"%s\" file=\"%s\" line=%d errors=%d\n", config, operator.buffer, argument.buffer, file, line, assay_config_error(config));
    }
}

/*******************************************************************************
 * SECTION
 ******************************************************************************/

void assay_parser_section_begin(void)
{
    action_begin(&section);
    action_begin(&key);
    action_end(&key);
}

void assay_parser_section_next(int ch)
{
    action_next(&section, ch);
}

void assay_parser_section_end(void)
{
    action_end(&section);
    if (debug) {
        DIMINUTO_LOG_DEBUG("assay_parser: section[%zu]=\"%s\"[%zu]\n", section.length, section.buffer, section.index);
    }
}

/*******************************************************************************
 * KEY
 ******************************************************************************/

void assay_parser_key_begin(void)
{
    action_begin(&key);
    action_begin(&value);
    action_end(&value);
}

void assay_parser_key_next(int ch)
{
    action_next(&key, ch);
}

void assay_parser_key_end(void)
{
    action_end(&key);
    if (debug) {
        DIMINUTO_LOG_DEBUG("assay_parser: key[%zu]=\"%s\"[%zu]\n", key.length, key.buffer, key.index);
    }
}

/*******************************************************************************
 * VALUE
 ******************************************************************************/

void assay_parser_value_begin(void)
{
   action_begin(&value);
}

void assay_parser_value_next(int ch)
{
    action_next(&value, ch);
}

void assay_parser_value_end(void)
{
    action_end(&value);
    if (!debug) {
        /* Do nothing. */
    } else if (!DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
        /* Do nothing. */
    } else if (diminuto_escape_printable(value.buffer)) {
        DIMINUTO_LOG_DEBUG("assay_parser: value[%zu]=\"%s\"[%zu]\n", value.length, value.buffer, value.index);
    } else {
        DIMINUTO_LOG_DEBUG("assay_parser: value[%zu]:\n", value.length);
        diminuto_dump(diminuto_log_stream(), value.buffer, value.index);
    }
 }

/*******************************************************************************
 * PROPERTY
 ******************************************************************************/

void assay_parser_property_assign(void)
{
    const char * name = ASSAY_SECTION_DEFAULT;

    if (section.buffer == (char *)0) {
        /* Do nothing. */
    } else if (section.index == 0) {
        /* Do nothing. */
    } else {
        name = section.buffer;
    }

    if (config != (assay_config_t *)0) {
        assay_config_write_binary(config, name, key.buffer, value.buffer, value.index);
    }
}

/*******************************************************************************
 * PARSER
 ******************************************************************************/

int assay_parser_debug(int enable)
{
    int prior;

    prior = debug;
    debug = enable;

    return prior;
}

assay_config_t * assay_parser_output(assay_config_t * cfp)
{
    assay_config_t * prior;

    prior = config;
    config = cfp;

    return prior;
}

const char * assay_parser_file(const char * path)
{
    const char * prior;

    prior = file;
    file = path;

    return prior;
}

int assay_parser_line(int origin)
{
    int prior;

    prior = line;
    line = origin;

    return prior;
}

void assay_parser_next(void)
{
    ++line;
}

void assay_parser_error(const char * msg)
{
    int errors = -1;
    extern char * assay_yytext;

    if (config != (assay_config_t *)0) {
        assay_config_error(config);
        errors = assay_config_errors(config);
    }

    DIMINUTO_LOG_WARNING("assay_parser_error: *%s* config=%p file=\"%s\" line=%d text=\"%s\" errors=%d\n", msg, config, file, line + 1, assay_yytext, errors);

    assay_parser_value_end();
    assay_parser_key_end();
    assay_parser_section_end();
    assay_parser_argument_end();
    assay_parser_operator_end();
}
