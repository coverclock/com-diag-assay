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
#include "com/diag/assay/assay_scanner.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    int token;
    const char * name;
    extern FILE * yyin;
    extern char * yytext;

    SETLOGMASK();

    assay_scanner_debug(!0);

    ASSERT((yyin = fopen("etc/test1.ini", "r")) != (FILE *)0);

    do {
        token = yylex();
        name = assay_scanner_token2name(token);
    } while (token != 0);

    ASSERT(fclose(yyin) == 0);

    EXIT();
}
