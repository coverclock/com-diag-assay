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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "com/diag/assay/assay_scanner.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    int token;
    const char * name;
    extern char * yytext;

    SETLOGMASK();

    do {
        token = yylex();
        name = assay_scanner_token2name(token);
        if (isprint(yylval)) {
        	DIMINUTO_LOG_INFORMATION("%s@%d: \"%s\"[%zu] '%c' %d %s\n", __FILE__, __LINE__, yytext, strlen(yytext), yylval, token, name);
        } else if (isprint(*yytext)) {
        	DIMINUTO_LOG_INFORMATION("%s@%d: \"%s\"[%zu] \\x%2.2x %d %s\n", __FILE__, __LINE__, yytext, strlen(yytext), yylval, token, name);
        } else {
        	DIMINUTO_LOG_INFORMATION("%s@%d: \\x%2.2x[%zu] \\x%2.2x %d %s\n", __FILE__, __LINE__, *yytext, strlen(yytext), yylval, token, name);
        }
    } while (token != 0);

    EXIT();
}
