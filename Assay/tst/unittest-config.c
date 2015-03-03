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
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"

static const char PATH0[] = "etc/test0.ini";
static const char PATH1[] = "etc/test1.ini";

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
    SETLOGMASK();

    if ((argc > 1) && (strcmp(argv[1], "-d") == 0)) {
        assay_scanner_debug(!0);
        assay_parser_debug(!0);
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
        int pipeline[2];
        pid_t pid;
        int status;
        ASSERT(pipe(pipeline) == 0);
        if ((pid = fork()) < 0) {
            ASSERT(pid >= 0);
        } else if (pid == 0) {
            close(pipeline[0]); /* This seems to be necessary to keep the parent from blocking. */
            assay_config_destroy(assay_config_export_stream_close(assay_config_import_file(assay_config_create(), PATH1), fdopen(pipeline[1], "w")));
            DIMINUTO_LOG_DEBUG("unittest-config: producer: exiting\n");
            exit(0);
#if 0
        } else if (pid > 0) {
            FILE * stream;
            int ch;
            close(pipeline[1]);
            ASSERT((stream = fdopen(pipeline[0], "r")) != (FILE *)0);
            while ((ch = fgetc(stream)) != EOF) { fputc(ch, stdout); }
            ASSERT(fclose(stream) == 0);
#endif
        } else {
            assay_config_t * cfp;
            const char * value;
            int sections;
            int properties;
            int rc;
            ASSERT(close(pipeline[1]) == 0); /* This seems to be necessary to keep the parent from blocking. */
            ASSERT((cfp = assay_config_import_stream_close(assay_config_create(), fdopen(pipeline[0], "r"))) != (assay_config_t *)0);
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
            ASSERT((rc = waitpid(pid, &status, 1)) >= 0); /* valgrind(1) affects the PID that is returned. */
            DIMINUTO_LOG_DEBUG("unittest-config: consumer: pid=%d rc=%d status=%d\n", pid, rc, status);
            STATUS();
        }
    }

    EXIT();
}
