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
#include <ctype.h>
#include "com/diag/assay/assay_scanner.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"

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
    case RB:        name = "RB";        break;
    case SC:        name = "SC";        break;
    case SP:        name = "SP";        break;
    default:        name = "ERR";       break;
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

    if (DIMINUTO_LOG_ENABLED(DIMINUTO_LOG_MASK_DEBUG)) {
        int printable = !0;
        size_t ii;
        size_t limit;
        limit = strlen(text);
        for (ii = 0; ii < limit; ++ii) {
            if (!isprint(text[ii])) {
                printable = 0;
                break;
            }
        }
        if (printable) {
            DIMINUTO_LOG_DEBUG("assay_scanner: value=0x%2.2x text=\"%s\"\n", value, text);
        } else {
            DIMINUTO_LOG_DEBUG("assay_scanner: value=0x%2.2x text:\n", value);
            diminuto_dump(diminuto_log_stream(), text, limit + 1);
        }
    }

    return value;
}
