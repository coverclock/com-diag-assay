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
#include "com/diag/assay/assay_parser.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, char ** argv)
{
    extern FILE * assay_yyin;

    SETLOGMASK();

    assay_parser_debug(!0);

    ASSERT((assay_yyin = fopen("etc/test0.ini", "r")) != (FILE *)0);

    do {
        assay_yyparse();
    } while (!feof(assay_yyin));

    ASSERT(fclose(assay_yyin) == 0);

    EXIT();
}
