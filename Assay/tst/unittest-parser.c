/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * This tests the interface between Assay and the Bison shift-reduce parser.
 *
 * Best if run under valgrind(1).
 *
 * TODO: test all of the parser call backs individually.
 */

#include <stdio.h>
#include "com/diag/assay/assay.h"
#include "com/diag/assay/assay_scanner_annex.h"
#include "com/diag/assay/assay_parser_annex.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    SETLOGMASK();

    {
        assay_config_t * cfp = 0;
        assay_scanner_lexical_t lxp = 0;
        FILE * stream;

        assay_parser_debug(!0);

        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT((stream = fopen("etc/test0.ini", "r")) != (FILE *)0);
        ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);

        do {
            DIMINUTO_LOG_DEBUG("unittest-parser: assay_parser_parse\n");
            ASSERT(assay_parser_parse(lxp) == 0);
        } while (!feof(stream));
        assay_parser_fini(lxp);

        assay_scanner_destroy(lxp);
        ASSERT(fclose(stream) == 0);
        assay_config_destroy(cfp);
    }

    EXIT();
}

