/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
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
#include "assay_scanner_annex.h"
#include "assay_parser_annex.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    SETLOGMASK();

    {
        int debug;
        assay_config_t * cfp;
        assay_scanner_lexical_t lxp;
        FILE * stream;
        const char * value;
        static const char BEFORE[] = "BEFORE";
        static const char AFTER[] = "AFTER";
        static const char before[] = "before";
        static const char after[] = "after";
        debug = assay_parser_debug(!0);
        ASSERT((stream = tmpfile()) != (FILE *)0);
        ASSERT(fprintf(stream, "%s=%s\n", BEFORE, before) >= 0);
        ASSERT(fputs(ASSAY_END_OF_TRANSMISSION, stream) >= 0);
        ASSERT(fprintf(stream, "%s=%s\n", AFTER, after) >= 0);
        ASSERT(fflush(stream) == 0);
        rewind(stream);
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);
        DIMINUTO_LOG_DEBUG("unittest-parser: assay_parser_parse\n");
        ASSERT(assay_parser_parse(lxp) == 0);
        assay_parser_fini(lxp);
        assay_scanner_destroy(lxp);
        ASSERT(fclose(stream) == 0);
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, BEFORE)) != (const char *)0) && (strcmp(value, before) == 0));
        EXPECT((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, AFTER)) == (const char *)0);
        assay_config_destroy(cfp);
        assay_parser_debug(debug);
        STATUS();
    }

    {
        int debug;
        assay_config_t * cfp;
        FILE * stream;
        assay_scanner_lexical_t lxp;
        int count = 0;
        debug = assay_parser_debug(!0);
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT((stream = fopen("etc/test0.ini", "r")) != (FILE *)0);
        ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);
        do {
            ASSERT(assay_parser_parse(lxp) == 0);
            ++count;
        } while (!feof(stream));
        ASSERT(count == 1);
        assay_parser_fini(lxp);
        assay_scanner_destroy(lxp);
        ASSERT(fclose(stream) == 0);
        assay_config_destroy(cfp);
        assay_parser_debug(debug);
        STATUS();
    }

    {
        int debug;
        assay_config_t * cfp;
        FILE * stream;
        assay_scanner_lexical_t lxp;
        int count = 0;
        debug = assay_parser_debug(!0);
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT((stream = fopen("etc/test1.ini", "r")) != (FILE *)0);
        ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);
        do {
            ASSERT(assay_parser_parse(lxp) == 0);
            ++count;
        } while (!feof(stream));
        ASSERT(count == 1);
        assay_parser_fini(lxp);
        assay_scanner_destroy(lxp);
        ASSERT(fclose(stream) == 0);
        assay_config_destroy(cfp);
        assay_parser_debug(debug);
        STATUS();
    }

    EXIT();
}

