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
#include <string.h>
#include "com/diag/assay/assay.h"
#include "assay_scanner_annex.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, int ** argv)
{
    SETLOGMASK();

    {
        const char * string;
        char buffer[sizeof("\\0377")];
        unsigned int in;
        unsigned int out;
        /*
         * This is just to convince myself that I actually understand how C
         * encodes octal and hexadecimal escape sequences, since that's what
         * my code is emulating.
         */
        ASSERT((unsigned char)'\0' == 0U);
        ASSERT((unsigned char)'\7' == 07U);
        ASSERT((unsigned char)'\77' == 077U);
        ASSERT((unsigned char)'\377' == 0377U);
        ASSERT((unsigned char)'\x0' == 0x0U);
        ASSERT((unsigned char)'\xf' == 0xfU);
        ASSERT((unsigned char)'\xF' == 0xFU);
        ASSERT((unsigned char)'\xff' == 0xffU);
        ASSERT((unsigned char)'\xFF' == 0xFFU);
        in = '\0';
        string = "\\0";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\a';
        string = "\\a";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\b';
        string = "\\b";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\t';
        string = "\\t";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\n';
        string = "\\n";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\v';
        string = "\\v";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\f';
        string = "\\f";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\r';
        string = "\\r";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        in = '\\';
        string = "\\\\";
        out = assay_scanner_text2value(string);
        DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, string, out);
        ASSERT(out == in);
        buffer[1] = '\0';
        for (in = ' '; in <= '~'; ++in) {
            if (in == '\\') { continue; }
            buffer[0] = in;
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: '%c' 0x%2.2x \"%s\" '%c' 0x%2.2x\n", in, in, buffer, out, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 07; ++in) {
            snprintf(buffer, sizeof(buffer), "\\%o", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0%o \"%s\" 0%o\n", in, buffer, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 077; ++in) {
            snprintf(buffer, sizeof(buffer), "\\%o", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0%o \"%s\" 0%o\n", in, buffer, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 0377; ++in) {
            snprintf(buffer, sizeof(buffer), "\\%o", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0%o \"%s\" 0%o\n", in, buffer, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 0xF; ++in) {
            snprintf(buffer, sizeof(buffer), "\\x%X", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%X \"%s\" 0x%X\n", in, buffer, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 0xFF; ++in) {
            snprintf(buffer, sizeof(buffer), "\\x%X", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%X \"%s\" 0x%X\n", in, buffer, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 0xf; ++in) {
            snprintf(buffer, sizeof(buffer), "\\x%x", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%x \"%s\" 0x%x\n", in, buffer, out);
            ASSERT(out == in);
        }
        for (in = 0; in <= 0xff; ++in) {
            snprintf(buffer, sizeof(buffer), "\\x%x", in);
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%x \"%s\" 0x%x\n", in, buffer, out);
            ASSERT(out == in);
        }
        buffer[0] = '\\';
        buffer[2] = '\0';
        for (in = ' '; in <= '~'; ++in) {
            if (('0' <= in) && (in <= '7')) { continue; }
            if (in == '\\') { continue; }
            if (in == 'a') { continue; }
            if (in == 'b') { continue; }
            if (in == 'f') { continue; }
            if (in == 'n') { continue; }
            if (in == 't') { continue; }
            if (in == 'r') { continue; }
            if (in == 'v') { continue; }
            if (in == 'x') { continue; }
            buffer[1] = in;
            out = assay_scanner_text2value(buffer);
            DIMINUTO_LOG_DEBUG("unittest-scanner: 0x%2.2x \"%s\" 0x%2.2x\n", in, buffer, out);
            ASSERT(out == in);
        }
        STATUS();
    }

    {
        int debug;
        assay_config_t * cfp;
        FILE * stream;
        assay_scanner_lexical_t lxp;
        assay_scanner_lvalue_t lval;
        int token;
        const char * name;
        debug = assay_scanner_debug(!0);
        ASSERT((stream = tmpfile()) != (FILE *)0);
        ASSERT(fputs(ASSAY_END_OF_TRANSMISSION, stream) >= 0);
        rewind(stream);
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);
        ASSERT((token = assay_scanner_scan(&lval, lxp)) == 0);
        ASSERT((name = assay_scanner_token2name(token)) != (const char *)0);
        assay_scanner_destroy(lxp);
        ASSERT(fclose(stream) == 0);
        assay_config_destroy(cfp);
        assay_scanner_debug(debug);
        STATUS();
    }

    {
        int debug;
        assay_config_t * cfp;
        FILE * stream;
        assay_scanner_lexical_t lxp;
        assay_scanner_lvalue_t lval;
        int token;
        const char * name;
        debug = assay_scanner_debug(!0);
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT((stream = fopen("etc/test1.ini", "r")) != (FILE *)0);
        ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);
        do {
            token = assay_scanner_scan(&lval, lxp);
            ASSERT((name = assay_scanner_token2name(token)) != (const char *)0);
        } while (token != 0);
        assay_scanner_destroy(lxp);
        ASSERT(fclose(stream) == 0);
        assay_config_destroy(cfp);
        assay_scanner_debug(debug);
        STATUS();
    }

    EXIT();
}
