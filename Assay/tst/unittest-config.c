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
#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"

static const char PATH0[] = "etc/test0.ini";
static const char PATH1[] = "etc/test1.ini";

static assay_config_t * import(const char * path)
{
    return assay_config_export_stream(assay_config_import_file(assay_config_create(), path), stderr);
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

    EXIT();
}
