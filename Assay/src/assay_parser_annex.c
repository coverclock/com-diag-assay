/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * This is the glue between the Bison/Yacc parser and Assay.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assay.h"
#include "assay_parser.h"
#include "assay_fixup.h"
#include "assay_scanner.h"
#include "assay_parser_annex.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_escape.h"
#include "com/diag/diminuto/diminuto_heap.h"

/*******************************************************************************
 * GLOBALS
 ******************************************************************************/

static int debug = 0;

/*******************************************************************************
 * CODE GENERATORS
 ******************************************************************************/

#define ASSAY_PARSER_ACTION_BEGIN(_ACTION_) \
    do { \
        if (lxp != (assay_scanner_lexical_t)0) { \
            assay_config_t * cfp; \
            cfp = (assay_config_t *)assay_scanner_yyget_extra((yyscan_t)lxp); \
            if (cfp != (assay_config_t *)0) { \
                assay_action_t * acp; \
                acp = &(cfp->_ACTION_); \
                do {

#define ASSAY_PARSER_ACTION_END \
                } while (0); \
            } \
        } \
    } while (0)

/*******************************************************************************
 * ACTION
 ******************************************************************************/

static void action_begin(assay_action_t * ap)
{
    if (ap->buffer == (char *)0) {
        ap->length = assay_buffer_default_size;
        ap->buffer = (char *)diminuto_heap_malloc(ap->length);
    }
    ap->index = 0;
}

static void action_next(assay_action_t * ap, int ch)
{
    if (ap->buffer == (char *)0) {
        action_begin(ap);
    }
    if (ap->index >= ap->length) {
        char * old;
        old = ap->buffer;
        ap->length *= 2;
        ap->buffer = (char *)diminuto_heap_malloc(ap->length);
        memcpy(ap->buffer, old, ap->index);
        diminuto_heap_free(old);
    }
    ap->buffer[ap->index++] = ch;
}

static void action_end(assay_action_t * ap)
{
    if ((ap->index <= 0) || (ap->buffer[ap->index - 1] != '\0')) {
        action_next(ap, '\0');
    }
}

static void action_fini(assay_action_t * ap)
{
	diminuto_heap_free(ap->buffer);
    ap->buffer = (char *)0;
    ap->length = 0;
    ap->index = 0;
}

/*******************************************************************************
 * OPERATOR
 ******************************************************************************/

void assay_parser_operator_begin(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        action_begin(acp);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_operator_next(assay_scanner_lexical_t lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_operator_end(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        action_end(acp);
        if (debug) {
            DIMINUTO_LOG_DEBUG("assay_parser: operator[%zu]=\"%s\"[%zu]\n", acp->length, acp->buffer, acp->index);
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * ARGUMENT
 ******************************************************************************/

void assay_parser_argument_begin(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(aaction);
        action_begin(acp);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_argument_next(assay_scanner_lexical_t lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(aaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_argument_end(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(aaction);
        action_end(acp);
        if (debug) {
            DIMINUTO_LOG_DEBUG("assay_parser: argument[%zu]=\"%s\"[%zu]\n", acp->length, acp->buffer, acp->index);
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * OPERATION
 ******************************************************************************/

void assay_parser_operation_execute(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        if (strcmp(acp->buffer, "include") == 0) {
            assay_config_import_file(cfp, cfp->aaction.buffer);
        } else if (strcmp(acp->buffer, "exec") == 0) {
            assay_config_import_command(cfp, cfp->aaction.buffer);
        } else {
            int errors;
            errors = assay_config_error(cfp);
            DIMINUTO_LOG_WARNING("assay_parser_operation_execute: *invalid* config=%p operator=\"%s\" argument=\"%s\" file=\"%s\" line=%d errors=%d\n", cfp, acp->buffer, cfp->aaction.buffer, cfp->file, cfp->lines, errors);
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * SECTION
 ******************************************************************************/

void assay_parser_section_begin(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(saction);
        action_begin(acp);
        action_begin(&(cfp->kaction));
        action_end(&(cfp->kaction));
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_section_next(assay_scanner_lexical_t lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(saction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_section_end(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(saction);
        action_end(acp);
        if (debug) {
            DIMINUTO_LOG_DEBUG("assay_parser: section[%zu]=\"%s\"[%zu]\n", acp->length, acp->buffer, acp->index);
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * KEY
 ******************************************************************************/

void assay_parser_key_begin(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(kaction);
        action_begin(acp);
        action_begin(&(cfp->vaction));
        action_end(&(cfp->vaction));
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_key_next(assay_scanner_lexical_t lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(kaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_key_end(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(kaction);
        action_end(acp);
        if (debug) {
            DIMINUTO_LOG_DEBUG("assay_parser: key[%zu]=\"%s\"[%zu]\n", acp->length, acp->buffer, acp->index);
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * VALUE
 ******************************************************************************/

void assay_parser_value_begin(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(vaction);
       action_begin(acp);
   ASSAY_PARSER_ACTION_END;
}

void assay_parser_value_next(assay_scanner_lexical_t lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(vaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_value_end(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(vaction);
        action_end(acp);
        if (!debug) {
            /* Do nothing. */
        } else if (!DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
            /* Do nothing. */
        } else if (diminuto_escape_printable(acp->buffer)) {
            DIMINUTO_LOG_DEBUG("assay_parser: value[%zu]=\"%s\"[%zu]\n", acp->length, acp->buffer, acp->index);
        } else {
            DIMINUTO_LOG_DEBUG("assay_parser: value[%zu]:\n", acp->length);
            diminuto_dump(diminuto_log_stream(), acp->buffer, acp->index);
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * PROPERTY
 ******************************************************************************/

void assay_parser_property_assign(assay_scanner_lexical_t lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(saction);
        const char * name = ASSAY_SECTION_DEFAULT;
        if (acp->buffer == (char *)0) {
            /* Do nothing. */
        } else if (acp->index == 0) {
            /* Do nothing. */
        } else if (acp->buffer[0] == '\0') {
            /* Do nothing. */
        } else {
            name = acp->buffer;
        }
        assay_config_write_binary(cfp, name, cfp->kaction.buffer, cfp->vaction.buffer, cfp->vaction.index);
    ASSAY_PARSER_ACTION_END;
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

int assay_parser_parse(assay_scanner_lexical_t lxp)
{
    return assay_parser_yyparse(lxp);
}

void assay_parser_next(assay_scanner_lexical_t lxp)
{
    assay_scanner_next(lxp);
}

void assay_parser_error(assay_scanner_lexical_t lxp, const char * msg)
{
    int errors = -1;
    if (lxp != (assay_scanner_lexical_t)0) {
        assay_config_t * cfp;
        cfp = (assay_config_t *)assay_scanner_yyget_extra((yyscan_t)lxp);
        if (cfp != (assay_config_t *)0) {
            const char * text;
            errors = assay_config_error(cfp);
            text = assay_scanner_yyget_text((yyscan_t)lxp);
            DIMINUTO_LOG_WARNING("assay_parser_error: *%s* scanner=%p config=%p file=\"%s\" line=%d text=\"%s\" errors=%d\n", msg, lxp, cfp, cfp->file, cfp->lines + 1, (text != (const char *)0) ? text : "", errors);
            action_end(&(cfp->vaction));
            action_end(&(cfp->kaction));
            action_end(&(cfp->saction));
            action_end(&(cfp->aaction));
            action_end(&(cfp->oaction));
        }
    }
    if (errors < 0) {
        DIMINUTO_LOG_WARNING("assay_parser_error: *%s*\n", msg);
    }
}

void assay_parser_fini(assay_scanner_lexical_t lxp)
{
    if (lxp != (assay_scanner_lexical_t)0) {
        assay_config_t * cfp;
        cfp = (assay_config_t *)assay_scanner_yyget_extra((yyscan_t)lxp);
        if (cfp != (assay_config_t *)0) {
            action_fini(&(cfp->vaction));
            action_fini(&(cfp->kaction));
            action_fini(&(cfp->saction));
            action_fini(&(cfp->aaction));
            action_fini(&(cfp->oaction));
        }
    }
}
