/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * This tests the interface between Assay and the Flex lexical scanner.
 */

#include <stdio.h>
#include "assay_parser.h"
#include "assay_fixup.h"
#include "assay_scanner.h"
#include "com/diag/assay/assay.h"
#include "com/diag/assay/assay_scanner_annex.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    assay_config_t * cfp = 0;
    yyscan_t scanner = 0;
    FILE * stream;
    YYSTYPE lval;
    int token = 0;
    const char * name = 0;

    SETLOGMASK();

    assay_scanner_debug(!0);

    ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);

    assay_scanner_yylex_init_extra(cfp, &scanner);
    ASSERT(scanner != (yyscan_t)0);

    ASSERT((stream = fopen("etc/test1.ini", "r")) != (FILE *)0);
    assay_scanner_yyset_in(stream , scanner);

    do {
        token = assay_scanner_yylex(&lval, scanner);
        name = assay_scanner_token2name(token);
    } while (token != 0);

    ASSERT(fclose(stream) == 0);

    assay_scanner_yylex_destroy(scanner);
    assay_config_destroy(cfp);

    EXIT();
}
