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
#include "com/diag/assay/assay.h"
#include "com/diag/assay/assay_scanner_annex.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    assay_config_t * cfp;
    FILE * stream;
    assay_scanner_lexical_t lxp;
    assay_scanner_lvalue_t lval;
    int token;
    const char * name;

    SETLOGMASK();

    assay_scanner_debug(!0);

    ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
    ASSERT((stream = fopen("etc/test1.ini", "r")) != (FILE *)0);
    ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);

    do {
        token = assay_scanner_scan(&lval, lxp);
        name = assay_scanner_token2name(token);
    } while (token != 0);

    assay_scanner_destroy(lxp);
    ASSERT(fclose(stream) == 0);
    assay_config_destroy(cfp);

    EXIT();
}
