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

int yyerror(char * msg)
{
    fprintf(stderr, "yyerror: \"%s\"\n", msg);
    return 0;
}

int main(int argc, char ** argv)
{
    extern FILE * yyin;

    SETLOGMASK();

    assay_parser_debug(!0);

    do {
        yyparse();
    } while (!feof(yyin));

    EXIT();
}
