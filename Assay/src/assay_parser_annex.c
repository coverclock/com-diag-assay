/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
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
#include "com/diag/assay/assay_parser_annex.h"
#include "com/diag/assay/assay_scanner_annex.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_escape.h"

/*******************************************************************************
 * GLOBALS
 ******************************************************************************/

static int debug = 0;

/*******************************************************************************
 * CODE GENERATORS
 ******************************************************************************/

#define ASSAY_PARSER_ACTION_BEGIN(_ACTION_) \
    do { \
        if (lxp != (void *)0) { \
            assay_config_t * cfp; \
            cfp = (assay_config_t *)assay_scanner_yyget_extra((void *)lxp); \
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
        ap->length = ASSAY_BUFFER_DEFAULT_SIZE;
        ap->buffer = (char *)malloc(ap->length);
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
        ap->buffer = (char *)malloc(ap->length);
        memcpy(ap->buffer, old, ap->index);
        free(old);
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
    free(ap->buffer);
    ap->buffer = (char *)0;
    ap->length = 0;
    ap->index = 0;
}

/*******************************************************************************
 * OPERATOR
 ******************************************************************************/

void assay_parser_operator_begin(void * lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        action_begin(acp);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_operator_next(void * lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_operator_end(void * lxp)
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

void assay_parser_argument_begin(void * lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(aaction);
        action_begin(acp);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_argument_next(void * lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(aaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_argument_end(void * lxp)
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

void assay_parser_operation_execute(void * lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(oaction);
        if (strcmp(acp->buffer, "include") == 0) {
            assay_config_import_file(cfp, cfp->aaction.buffer);
        } else if (strcmp(acp->buffer, "exec") == 0) {
            assay_config_import_command(cfp, cfp->aaction.buffer);
        } else {
            assay_config_error(cfp);
            DIMINUTO_LOG_WARNING("assay_parser_operation_execute: *invalid* config=%p operator=\"%s\" argument=\"%s\" file=\"%s\" line=%d errors=%d\n", cfp, acp->buffer, cfp->aaction.buffer, cfp->file, cfp->line, assay_config_error(cfp));
        }
    ASSAY_PARSER_ACTION_END;
}

/*******************************************************************************
 * SECTION
 ******************************************************************************/

void assay_parser_section_begin(void * lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(saction);
        action_begin(acp);
        action_begin(&(cfp->kaction));
        action_end(&(cfp->kaction));
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_section_next(void * lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(saction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_section_end(void * lxp)
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

void assay_parser_key_begin(void * lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(kaction);
        action_begin(acp);
        action_begin(&(cfp->vaction));
        action_end(&(cfp->vaction));
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_key_next(void * lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(kaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_key_end(void * lxp)
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

void assay_parser_value_begin(void * lxp)
{
    ASSAY_PARSER_ACTION_BEGIN(vaction);
       action_begin(acp);
   ASSAY_PARSER_ACTION_END;
}

void assay_parser_value_next(void * lxp, int ch)
{
    ASSAY_PARSER_ACTION_BEGIN(vaction);
        action_next(acp, ch);
    ASSAY_PARSER_ACTION_END;
}

void assay_parser_value_end(void * lxp)
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

void assay_parser_property_assign(void * lxp)
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

void assay_parser_next(void * lxp)
{
    if (lxp != (void *)0) {
        assay_config_t * cfp;
        cfp = (assay_config_t *)assay_scanner_yyget_extra((void *)lxp);
        if (cfp != (assay_config_t *)0) {
            ++cfp->line;
        }
    }
}

void assay_parser_error(void * lxp, const char * msg)
{
    int errors = -1;
    if (lxp != (void *)0) {
        assay_config_t * cfp;
        cfp = (assay_config_t *)assay_scanner_yyget_extra((void *)lxp);
        if (cfp != (assay_config_t *)0) {
            const char * text;
            assay_config_error(cfp);
            errors = assay_config_errors(cfp);
            text = assay_scanner_yyget_text((void *)lxp);
            if (text == (const char *)0) { text = ""; }
            DIMINUTO_LOG_WARNING("assay_parser_error: *%s* scanner=%p config=%p file=\"%s\" line=%d text=\"%s\" errors=%d\n", msg, lxp, cfp, cfp->file, cfp->line + 1, text, errors);
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

void assay_parser_fini(void * lxp)
{
    if (lxp != (void *)0) {
        assay_config_t * cfp;
        cfp = (assay_config_t *)assay_scanner_yyget_extra((void *)lxp);
        if (cfp != (assay_config_t *)0) {
            action_fini(&(cfp->vaction));
            action_fini(&(cfp->kaction));
            action_fini(&(cfp->saction));
            action_fini(&(cfp->aaction));
            action_fini(&(cfp->oaction));
        }
    }
}
