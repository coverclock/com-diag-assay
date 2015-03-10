//* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Diminuto.html<BR>
 *
 * This tests Assay with the Flex and Bison components.
 *
 * Best if run under valgrind(1).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "com/diag/assay/assay.h"
#include "com/diag/assay/assay_scanner_annex.h"
#include "com/diag/assay/assay_parser_annex.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"
#include "com/diag/diminuto/diminuto_ipc.h"
#include "com/diag/diminuto/diminuto_buffer.h"
#include "com/diag/diminuto/diminuto_heap.h"
#include "com/diag/diminuto/diminuto_string.h"

static const char PATH0[] = "etc/test0.ini";
static const char PATH1[] = "etc/test1.ini";
static const char PATH2[] = "etc/Message1.txt";
static const char PATH3[] = "etc/Message3.txt";

static assay_config_t * import(const char * path)
{
    return assay_config_export_stream(assay_config_import_file(assay_config_create(), path), stdout);
}

static void census(assay_config_t * cfp, int * sectionsp, int * propertiesp) {
    assay_section_t * scp;
    assay_property_t * prp;
    *sectionsp = 0;
    *propertiesp = 0;
    for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
        ++(*sectionsp);
        for (prp = assay_property_first(scp); prp != (assay_property_t *)0; prp = assay_property_next(prp)) {
            ++(*propertiesp);
        }
    }
}

int main(int argc, char ** argv)
{
    int debug = 0;

    SETLOGMASK();

    if (argc > 1) {
        debug = atoi(argv[1]);
        DIMINUTO_LOG_DEBUG("%s: debug=\"%s\"=%d\n", argv[0], argv[1], debug);
    }

    {
        assay_config_t * cfp;
        const char * value;
        int sections;
        int properties;
        ASSERT((cfp = import(PATH0)) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        census(cfp, &sections, &properties);
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
        assay_config_log(cfp);
        EXPECT(sections == 7);
        EXPECT(properties == 16);
        EXPECT(assay_config_errors(cfp) == 2);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        assay_config_t * cfp;
        const char * value;
        int sections;
        int properties;
        ASSERT((cfp = import(PATH1)) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        census(cfp, &sections, &properties);
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Bad", "KeywordTen")) != (const char *)0) && (strcmp(value, "10@ten.org") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Test2Section1", "KeywordEleven")) != (const char *)0) && (strcmp(value, "11.11.11.11") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section 4.5", "keyword12")) != (const char *)0) && (strcmp(value, "Twelve!") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD61")) != (const char *)0) && (strcmp(value, "VALUE61") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD62")) != (const char *)0) && (strcmp(value, "VALUE62") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD63")) != (const char *)0) && (strcmp(value, "VALUE63") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD64")) != (const char *)0) && (strcmp(value, "VALUE64") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD65")) != (const char *)0) && (strcmp(value, "VALUE65") == 0));
        EXPECT(sections == 10);
        EXPECT(properties == 24);
        EXPECT(assay_config_errors(cfp) == 6);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        FILE * stream;
        ASSERT((stream = tmpfile()) != (FILE *)0);
        {
            assay_config_t * cfp;
            const char * value;
            int sections;
            int properties;
            ASSERT((cfp = import(PATH0)) != (assay_config_t *)0);
            ASSERT(assay_config_audit(cfp) == (void *)0);
            census(cfp, &sections, &properties);
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
            EXPECT(sections == 7);
            EXPECT(properties == 16);
            EXPECT(assay_config_errors(cfp) == 2);
            ASSERT(assay_config_export_stream(cfp, stream) == cfp);
            assay_config_destroy(cfp);
        }
        rewind(stream);
        {
            assay_config_t * cfp;
            const char * value;
            int sections;
            int properties;
            ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
            ASSERT(assay_config_import_stream(cfp, stream) == cfp);
            ASSERT(assay_config_audit(cfp) == (void *)0);
            census(cfp, &sections, &properties);
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
            EXPECT(sections == 7);
            EXPECT(properties == 16);
            EXPECT(assay_config_errors(cfp) == 0);
            assay_config_destroy(cfp);
        }
        ASSERT(fclose(stream) == 0);
        STATUS();
    }

    {
        FILE * stream;
        static const char KEY[] = "KEY";
        unsigned int in;
        size_t count;
        assay_config_t * cfp;
        const unsigned char * value;
        size_t length;
        unsigned int out;
        ASSERT((stream = tmpfile()) != (FILE *)0);
        fprintf(stream, "%s=\\\n", KEY);
        count = 0;
        fprintf(stream, "\\a\\\n"); ++count;
        fprintf(stream, "\\b\\\n"); ++count;
        fprintf(stream, "\\t\\\n"); ++count;
        fprintf(stream, "\\n\\\n"); ++count;
        fprintf(stream, "\\v\\\n"); ++count;
        fprintf(stream, "\\f\\\n"); ++count;
        fprintf(stream, "\\r\\\n"); ++count;
        for (in = ' '; in <= '~'; ++in) {
            if (in == ';') { continue; }
            if (in == '\\') { continue; }
            fprintf(stream, "%c\\\n", in);
            ++count;
        }
        for (in = 0; in <= 07; ++in) {
            fprintf(stream, "\\%o\\\n", in);
            ++count;
        }
        for (in = 0; in <= 077; ++in) {
            fprintf(stream, "\\%o\\\n", in);
            ++count;
        }
        for (in = 0; in <= 0377; ++in) {
            fprintf(stream, "\\%o\\\n", in);
            ++count;
        }
        for (in = 0; in <= 0xF; ++in) {
            fprintf(stream, "\\x%X\\\n", in);
            ++count;
        }
        for (in = 0; in <= 0xFF; ++in) {
            fprintf(stream, "\\x%X\\\n", in);
            ++count;
        }
        for (in = 0; in <= 0xf; ++in) {
            fprintf(stream, "\\x%x\\\n", in);
            ++count;
        }
        for (in = 0; in <= 0xff; ++in) {
            fprintf(stream, "\\x%x\\\n", in);
            ++count;
        }
        for (in = ' '; in <= '~'; ++in) {
            if (('0' <= in) && (in <= '7')) { continue; }
            if (in == 'a') { continue; }
            if (in == 'b') { continue; }
            if (in == 'f') { continue; }
            if (in == 'n') { continue; }
            if (in == 't') { continue; }
            if (in == 'r') { continue; }
            if (in == 'v') { continue; }
            if (in == 'x') { continue; }
            fprintf(stream, "\\%c\\\n", in); /* Includes semicolon and backslash. */
            ++count;
        }
        fprintf(stream, ";\n"); ++count; /* For trailing NUL. */
        rewind(stream);
        while (!0) {
            in = fgetc(stream);
            if (in == EOF) { break; }
            fputc(in, stdout);
        }
        rewind(stream);
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_import_stream(cfp, stream) == cfp);
        ASSERT(assay_config_export_stream(cfp, stdout) == cfp);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT((value = assay_config_read_binary(cfp, ASSAY_SECTION_DEFAULT, KEY, &length)) != (const unsigned char * )0);
        ASSERT(length == count);
        EXPECT((out = *(value++)) == '\a');
        EXPECT((out = *(value++)) == '\b');
        EXPECT((out = *(value++)) == '\t');
        EXPECT((out = *(value++)) == '\n');
        EXPECT((out = *(value++)) == '\v');
        EXPECT((out = *(value++)) == '\f');
        EXPECT((out = *(value++)) == '\r');
        for (in = ' '; in <= '~'; ++in) {
            if (in == ';') { continue; }
            if (in == '\\') { continue; }
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 07; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 077; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 0377; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 0xF; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 0xFF; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 0xf; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = 0; in <= 0xff; ++in) {
            EXPECT((out = *(value++)) == in);
        }
        for (in = ' '; in <= '~'; ++in) {
            if (('0' <= in) && (in <= '7')) { continue; }
            if (in == 'a') { continue; }
            if (in == 'b') { continue; }
            if (in == 'f') { continue; }
            if (in == 'n') { continue; }
            if (in == 't') { continue; }
            if (in == 'r') { continue; }
            if (in == 'v') { continue; }
            if (in == 'x') { continue; }
            EXPECT((out = *(value++)) == in);
        }
        EXPECT((out = *(value++)) == '\0');
        EXPECT(assay_config_errors(cfp) == 0);
        assay_config_log(cfp);
        assay_config_destroy(cfp);
        ASSERT(fclose(stream) == 0);
        STATUS();
    }

    {
        FILE * stream;
        ASSERT((stream = tmpfile()) != (FILE *)0);
        {
            assay_config_destroy(assay_config_export_stream(assay_config_import_file(assay_config_create(), PATH1), stream));
        }
        rewind(stream);
        {
            assay_config_t * cfp;
            const char * value;
            int sections;
            int properties;
            ASSERT((cfp = assay_config_import_stream(assay_config_create(), stream)) != (assay_config_t *)0);
            ASSERT(assay_config_audit(cfp) == (void *)0);
            census(cfp, &sections, &properties);
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "KeywordTen")) != (const char *)0) && (strcmp(value, "10@ten.org") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Test2Section1", "KeywordEleven")) != (const char *)0) && (strcmp(value, "11.11.11.11") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section 4.5", "keyword12")) != (const char *)0) && (strcmp(value, "Twelve!") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD61")) != (const char *)0) && (strcmp(value, "VALUE61") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD62")) != (const char *)0) && (strcmp(value, "VALUE62") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD63")) != (const char *)0) && (strcmp(value, "VALUE63") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD64")) != (const char *)0) && (strcmp(value, "VALUE64") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD65")) != (const char *)0) && (strcmp(value, "VALUE65") == 0));
            EXPECT(sections == 10);
            EXPECT(properties == 24);
            EXPECT(assay_config_errors(cfp) == 0);
            assay_config_destroy(cfp);
        }
        ASSERT(fclose(stream) == 0);
        STATUS();
    }

    {
        int pipeline[2][2];
        pid_t pid;
        static const char ACK = 0xa5;
        ASSERT(pipe(pipeline[0]) == 0); /* [0][0]=ConsumerRead, [0][1]=ProducerWrite */
        ASSERT(pipe(pipeline[1]) == 0); /* [1][0]=ProducerRead, [1][1]=ConsumerWrite */
#define CONSUMER_READ pipeline[0][0]
#define PRODUCER_WRITE pipeline[0][1]
#define PRODUCER_READ pipeline[1][0]
#define CONSUMER_WRITE pipeline[1][1]
        DIMINUTO_LOG_DEBUG("unittest-config: CONSUMER_READ=%d PRODUCER_WRITE=%d\n", CONSUMER_READ, PRODUCER_WRITE);
        DIMINUTO_LOG_DEBUG("unittest-config: PRODUCER_READ=%d CONSUMER_WRITE=%d\n", PRODUCER_READ, CONSUMER_WRITE);
        pid = fork();
        if (pid == 0) {
            ASSERT(close(CONSUMER_READ) == 0);
            ASSERT(close(CONSUMER_WRITE) == 0);
        } else if (pid > 0) {
            ASSERT(close(PRODUCER_READ) == 0);
            ASSERT(close(PRODUCER_WRITE) == 0);
        } else if (pid < 0) {
            /* Do nothing. */
        }
        if (pid < 0) {
            ASSERT(pid >= 0);
        } else if (pid == 0) {
            char acknowledgement = ~ACK;
            assay_config_destroy(assay_config_export_stream_close(assay_config_import_file(assay_config_create(), PATH1), fdopen(dup(PRODUCER_WRITE), "w")));
            ASSERT(read(PRODUCER_READ, &acknowledgement, sizeof(acknowledgement)) == sizeof(acknowledgement));
            ASSERT(acknowledgement == ACK);
            ASSERT(close(PRODUCER_READ) == 0);
            ASSERT(close(PRODUCER_WRITE) == 0);
            DIMINUTO_LOG_DEBUG("unittest-config: producer: exiting\n");
            EXIT();
        } else if (debug == 1) {
            FILE * stream;
            int ch;
            char ackowledge = ACK;
            ASSERT((stream = fdopen(CONSUMER_READ, "r")) != (FILE *)0);
            while ((ch = fgetc(stream)) != EOF) {
                if (ch == ASSAY_END_OF_TRANSMISSION[0]) {
                    fputs("^D\n", stdout);
                    break;
                }
                fputc(ch, stdout);
            }
            ASSERT(write(CONSUMER_WRITE, &ackowledge, sizeof(ackowledge)) == sizeof(ackowledge));
            ASSERT(close(CONSUMER_WRITE) == 0);
            ASSERT(fclose(stream) == 0);
            STATUS();
        } else if (debug == 2) {
            int debugs;
            int debugp;
            assay_config_t * cfp;
            FILE * stream;
            assay_scanner_lexical_t lxp;
            char ackowledge = ACK;
            debugs = assay_scanner_debug(!0);
            debugp = assay_parser_debug(!0);
            ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
            ASSERT((stream = fdopen(CONSUMER_READ, "r")) != (FILE *)0);
            ASSERT((lxp = assay_scanner_create(cfp, stream)) != (void *)0);
            ASSERT(assay_parser_parse(lxp) == 0);
            assay_parser_fini(lxp);
            assay_scanner_destroy(lxp);
            ASSERT(write(CONSUMER_WRITE, &ackowledge, sizeof(ackowledge)) == sizeof(ackowledge));
            ASSERT(close(CONSUMER_WRITE) == 0);
            ASSERT(fclose(stream) == 0);
            ASSERT(assay_config_export_stream(cfp, stdout) == cfp);
            assay_config_destroy(cfp);
            assay_parser_debug(debugp);
            assay_scanner_debug(debugs);
            STATUS();
        } else {
            assay_config_t * cfp;
            const char * value;
            int sections;
            int properties;
            char ackowledge = ACK;
            int rc;
            int status;
            ASSERT((cfp = assay_config_import_stream_close(assay_config_create(), fdopen(dup(CONSUMER_READ), "r"))) != (assay_config_t *)0);
            ASSERT(assay_config_audit(cfp) == (void *)0);
            census(cfp, &sections, &properties);
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "KeywordTen")) != (const char *)0) && (strcmp(value, "10@ten.org") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Test2Section1", "KeywordEleven")) != (const char *)0) && (strcmp(value, "11.11.11.11") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section 4.5", "keyword12")) != (const char *)0) && (strcmp(value, "Twelve!") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD61")) != (const char *)0) && (strcmp(value, "VALUE61") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD62")) != (const char *)0) && (strcmp(value, "VALUE62") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD63")) != (const char *)0) && (strcmp(value, "VALUE63") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD64")) != (const char *)0) && (strcmp(value, "VALUE64") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD65")) != (const char *)0) && (strcmp(value, "VALUE65") == 0));
            EXPECT(sections == 10);
            EXPECT(properties == 24);
            EXPECT(assay_config_errors(cfp) == 0);
            assay_config_destroy(cfp);
            ASSERT(write(CONSUMER_WRITE, &ackowledge, sizeof(ackowledge)) == sizeof(ackowledge));
            ASSERT((rc = waitpid(pid, &status, 1)) >= 0); /* valgrind(1) affects the PID that is returned. */
            ASSERT(close(CONSUMER_READ) == 0);
            ASSERT(close(CONSUMER_WRITE) == 0);
            DIMINUTO_LOG_DEBUG("unittest-config: consumer: reaped pid=%d rc=%d status=%d\n", pid, rc, status);
            STATUS();
        }
    }

    {
        static const diminuto_port_t PORT = 0xfff0;
        static const char ACK = 0xa5;
        int rendezvous;
        pid_t pid;
        ASSERT((rendezvous = diminuto_ipc_stream_provider(PORT)) >= 0);
        DIMINUTO_LOG_DEBUG("unittest-config: rendezvous=%d\n", rendezvous);
        pid = fork();
        if (pid < 0) {
            ASSERT(pid >= 0);
        } else if (pid == 0) {
            int producer;
            FILE * stream;
            char acknowledgement = ~ACK;
            ASSERT((producer = diminuto_ipc_stream_consumer(diminuto_ipc_address("localhost"), PORT)) >= 0);
            DIMINUTO_LOG_DEBUG("unittest-config: producer=%d\n", producer);
            assay_config_destroy(assay_config_export_stream_send(assay_config_import_file(assay_config_create(), PATH1), stream = fdopen(producer, "w")));
            EXPECT(read(producer, &acknowledgement, sizeof(acknowledgement)) == sizeof(acknowledgement));
            EXPECT(acknowledgement == ACK);
            EXPECT(fclose(stream) == 0);
            DIMINUTO_LOG_DEBUG("unittest-config: producer: exiting\n");
            EXIT();
        } else {
            int consumer;
            diminuto_ipv4_t address;
            diminuto_port_t port;
            FILE * stream;
            assay_config_t * cfp;
            const char * value;
            int sections;
            int properties;
            char ackowledge = ACK;
            int rc;
            int status;
            address = 0;
            port = (diminuto_port_t)-1;
            ASSERT((consumer = diminuto_ipc_stream_accept(rendezvous, &address, &port)) >= 0);
            DIMINUTO_LOG_DEBUG("unittest-config: consumer=%d\n", consumer);
            EXPECT(address != 0);
            EXPECT(port != (diminuto_port_t)-1);
            EXPECT(port != PORT);
            ASSERT((cfp = assay_config_import_stream(assay_config_create(), stream = fdopen(consumer, "r"))) != (assay_config_t *)0);
            ASSERT(assay_config_audit(cfp) == (void *)0);
            census(cfp, &sections, &properties);
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Bad", "KeywordTen")) != (const char *)0) && (strcmp(value, "10@ten.org") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Test2Section1", "KeywordEleven")) != (const char *)0) && (strcmp(value, "11.11.11.11") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section 4.5", "keyword12")) != (const char *)0) && (strcmp(value, "Twelve!") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD61")) != (const char *)0) && (strcmp(value, "VALUE61") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD62")) != (const char *)0) && (strcmp(value, "VALUE62") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD63")) != (const char *)0) && (strcmp(value, "VALUE63") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD64")) != (const char *)0) && (strcmp(value, "VALUE64") == 0));
            EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD65")) != (const char *)0) && (strcmp(value, "VALUE65") == 0));
            EXPECT(sections == 10);
            EXPECT(properties == 24);
            EXPECT(assay_config_errors(cfp) == 0);
            assay_config_destroy(cfp);
            ASSERT(write(consumer, &ackowledge, sizeof(ackowledge)) == sizeof(ackowledge));
            ASSERT((rc = waitpid(pid, &status, 1)) >= 0); /* valgrind(1) affects the PID that is returned. */
            ASSERT(fclose(stream) == 0);
            EXPECT(diminuto_ipc_close(rendezvous) >= 0);
            DIMINUTO_LOG_DEBUG("unittest-config: consumer: reaped pid=%d rc=%d status=%d\n", pid, rc, status);
            STATUS();
        }
    }
    {
        assay_config_t * cfp;
        const char * value;
        int sections;
        int properties;
        ASSERT((cfp = assay_config_import_file(assay_config_create(), PATH2)) != (assay_config_t *)0);
        EXPECT(assay_config_errors(cfp) == 0);
        census(cfp, &sections, &properties);
        EXPECT(sections == 2);
        EXPECT(properties == 6);
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint1", "FieldA")) != (const char *)0) && (strcmp(value, "e") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint1", "FieldB")) != (const char *)0) && (strcmp(value, "b") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint1", "FieldC")) != (const char *)0) && (strcmp(value, "f") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint2", "FieldA")) != (const char *)0) && (strcmp(value, "c") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint2", "FieldB")) != (const char *)0) && (strcmp(value, "g") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint2", "FieldC")) != (const char *)0) && (strcmp(value, "h") == 0));
        ASSERT((cfp = assay_config_import_file(cfp, PATH3)) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        EXPECT(assay_config_errors(cfp) == 0);
        census(cfp, &sections, &properties);
        EXPECT(sections == 3);
        EXPECT(properties == 10);
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint1", "FieldA")) != (const char *)0) && (strcmp(value, "e") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint1", "FieldB")) != (const char *)0) && (strcmp(value, "i") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint1", "FieldC")) != (const char *)0) && (strcmp(value, "j") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint2", "FieldA")) != (const char *)0) && (strcmp(value, "c") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint2", "FieldB")) != (const char *)0) && (strcmp(value, "g") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint2", "FieldC")) != (const char *)0) && (strcmp(value, "h") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint3", "FieldA")) != (const char *)0) && (strcmp(value, "k") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint3", "FieldB")) != (const char *)0) && (strcmp(value, "l") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint3", "FieldC")) != (const char *)0) && (strcmp(value, "m") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Endpoint3", "FieldD")) != (const char *)0) && (strcmp(value, "n") == 0));
        assay_config_destroy(cfp);
    }

    {
        assay_config_t * cfp;
        const char * value;
        void * pointer;
        int sections;
        int properties;
        int debug;
        /**/
        debug = diminuto_buffer_debug(!0);
        ASSERT(diminuto_heap_malloc_set(diminuto_buffer_malloc) == malloc);
        ASSERT(diminuto_heap_free_set(diminuto_buffer_free) == free);
        ASSERT(diminuto_string_strdup_set(diminuto_buffer_strdup) == strdup);
        /**/
        ASSERT((cfp = import(PATH1)) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        census(cfp, &sections, &properties);
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general1")) != (const char *)0) && (strcmp(value, "This is a general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general2")) != (const char *)0) && (strcmp(value, "This is another general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, "general3")) != (const char *)0) && (strcmp(value, "This is yet another general parameter.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r#=:[]\\!\xa\xbc\7\77\377") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Bad", "keyword8")) != (const char *)0) && (strcmp(value, "\"value 8\"") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Bad", "KeywordTen")) != (const char *)0) && (strcmp(value, "10@ten.org") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Test2Section1", "KeywordEleven")) != (const char *)0) && (strcmp(value, "11.11.11.11") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section 4.5", "keyword12")) != (const char *)0) && (strcmp(value, "Twelve!") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword9")) != (const char *)0) && (strcmp(value, "#=:[];") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "Section5", "keyword10")) != (const char *)0) && (strcmp(value, "12345678") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD61")) != (const char *)0) && (strcmp(value, "VALUE61") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD62")) != (const char *)0) && (strcmp(value, "VALUE62") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD63")) != (const char *)0) && (strcmp(value, "VALUE63") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD64")) != (const char *)0) && (strcmp(value, "VALUE64") == 0));
        EXPECT(((value = assay_config_read_string(cfp, "SectionSix", "KEYWORD65")) != (const char *)0) && (strcmp(value, "VALUE65") == 0));
        EXPECT(sections == 10);
        EXPECT(properties == 24);
        EXPECT(assay_config_errors(cfp) == 6);
        assay_config_destroy(cfp);
        /**/
        diminuto_buffer_log();
        diminuto_buffer_fini();
        diminuto_heap_malloc_set((diminuto_heap_malloc_func_t *)0);
        diminuto_heap_free_set((diminuto_heap_free_func_t *)0);
        diminuto_string_strdup_set((diminuto_string_strdup_func_t *)0);
        pointer = diminuto_heap_malloc(1);
        ASSERT(pointer != (const char *)0);
        diminuto_heap_free(pointer);
        diminuto_buffer_debug(debug);
        /**/
        STATUS();
    }

    EXIT();
}
