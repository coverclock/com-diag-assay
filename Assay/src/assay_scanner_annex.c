/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * This is the glue between the Flex/Lexx scanner and Assay.
 */

#include <string.h>
#include <stdio.h>
#include "assay.h"
#include "assay_parser.h"
#include "assay_fixup.h"
#include "assay_scanner.h"
#include "assay_scanner_annex.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"
#include "com/diag/diminuto/diminuto_fd.h"

static int debug = 0;

int assay_scanner_debug(int enable)
{
    int prior;

    prior = debug;
    debug = enable;

    return prior;
}

static int scanner_stream_isinteractive(FILE * stream)
{
    diminuto_fs_type_t type;
    int interactive;

    /*
     * Here we try to determine what kind of file stream we have, batch
     * (like a regular file) or interactive (like a socket, pipe, or
     * character device). The generated scanner must handle the I/O
     * differently depending on the kind of input stream. The batch option
     * is more efficient for files, but will cause the application to hang on
     * interactive devices. N.B. the classifications of the input stream
     * below deliberately may not match those for classifying the output stream.
     */

    type = diminuto_fd_type(fileno(stream));
    switch (type) {
    case DIMINUTO_FS_TYPE_TTY:
    case DIMINUTO_FS_TYPE_SOCKET:
    case DIMINUTO_FS_TYPE_CHARACTERDEV:
    case DIMINUTO_FS_TYPE_FIFO:
        interactive = !0;
        break;
    case DIMINUTO_FS_TYPE_UNKNOWN:
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
 * This isn't part of the public API but is exposed none the less just in case.
 */
assay_scanner_lexical_t assay_scanner_create_generic(void * cfp, FILE * stream, int interactive)
{
    yyscan_t scanner;
    extern void assay_scanner_yy_set_interactive(void *, int); /* Not part of the public API. */

    assay_scanner_yylex_init_extra(cfp, &scanner);
    assay_scanner_yyset_in(stream , scanner);
    assay_scanner_yy_set_interactive(scanner, interactive);

    return scanner;
}

assay_scanner_lexical_t assay_scanner_create(void * cfp, FILE * stream)
{
    return assay_scanner_create_generic(cfp, stream, scanner_stream_isinteractive(stream));
}

void assay_scanner_destroy(assay_scanner_lexical_t lxp)
{
    assay_scanner_yylex_destroy((yyscan_t)lxp);
}

int assay_scanner_scan(assay_scanner_lvalue_t * lvaluep, assay_scanner_lexical_t lxp)
{
    return assay_scanner_yylex((YYSTYPE *)lvaluep, (yyscan_t)lxp);
}

int assay_scanner_wrap(assay_scanner_lexical_t lxp)
{
    return 1; /* Normally I'd use !0 here but the Flex docs explicitly specify 1. */
}

void assay_scanner_next(assay_scanner_lexical_t lxp)
{
    if (lxp != (assay_scanner_lexical_t)0) {
        assay_config_t * cfp;
        cfp = (assay_config_t *)assay_scanner_yyget_extra((yyscan_t)lxp);
        if (cfp != (assay_config_t *)0) {
            assay_config_line(cfp);
        }
    }
}

const char * assay_scanner_token2name(int token)
{
    const char * name = (const char *)0;

    switch (token) {
    case END:       name = "END";       break;
    case CH:        name = "CH";        break;
    case EOL:       name = "EOL";       break;
    case EQ:        name = "EQ";        break;
    case ESC:       name = "ESC";       break;
    case LB:        name = "LB";        break;
    case OT:        name = "OT";        break;
    case RB:        name = "RB";        break;
    case SC:        name = "SC";        break;
    case SP:        name = "SP";        break;
    default:        name = "ERR";       break;
    }

    if (debug) {
        DIMINUTO_LOG_DEBUG("assay_scanner: token=%d symbol=%s\n", token, name);
    }

    return name;
}

int assay_scanner_text2value(const char * text)
{
    unsigned int value;

    if (text[0] != '\\') {
        value = text[0];
    } else if (text[1] == 'x') {
        value = strtoul(&(text[2]), (char **)0, 16);
    } else if (('0' <= text[1]) && (text[1] <= '7')) {
        value = strtoul(&(text[1]), (char **)0, 8);
    } else {
        switch (text[1]) {
        case 'a': value = '\a';      break;
        case 'b': value = '\b';      break;
        case 't': value = '\t';      break;
        case 'n': value = '\n';      break;
        case 'v': value = '\v';      break;
        case 'f': value = '\f';      break;
        case 'r': value = '\r';      break;
        default:  value = text[1];   break;
        }
    }

    if (!debug) {
        /* Do nothing. */
    } else if (!DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
        /* Do nothing. */
    } else if (diminuto_escape_printable(text)) {
        DIMINUTO_LOG_DEBUG("assay_scanner: value=0x%2.2x text=\"%s\"\n", value, text);
    } else {
        DIMINUTO_LOG_DEBUG("assay_scanner: value=0x%2.2x text:\n", value);
        diminuto_dump(diminuto_log_stream(), text, strlen(text) + 1);
    }

    return value;
}
