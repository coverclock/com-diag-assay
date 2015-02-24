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
#include <string.h>
#include "com/diag/assay/assay_scanner.h"
#include "com/diag/assay/assay_parser.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"

static int debug = 0;

int assay_scanner_debug(int enable)
{
    int prior;

    prior = debug;
    debug = enable;

    return prior;
}

FILE * assay_scanner_input(FILE * fp)
{
    extern FILE * assay_yyin;
    FILE * prior;

    prior = assay_yyin;
    if (fp != prior) {
        //yyless(0);
    	assay_yyin = fp;
        assay_yyrestart(fp);
    }

    return prior;
}

const char * assay_scanner_token2name(int token)
{
    const char * name = (const char *)0;

    switch (token) {
    case 0:         name = "END";       break;
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
    unsigned int value = 0;

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
        default:  value = text[1]; break;
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

int assay_scanner_wrap()
{
    return 1;
}
