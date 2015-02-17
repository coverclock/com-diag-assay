//* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Diminuto.html<BR>
 */

#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/diminuto/diminuto_escape.h"
#include <stdio.h>
#include <string.h>

static const char PATH1[] = "etc/test1.ini";

static assay_config_t * import(const char * path)
{
    assay_config_t * cfp;
    cfp = assay_config_load_file(assay_config_create(), path);
    printf("%s\n", path);
    if (cfp != (assay_config_t *)0) {
        assay_section_t * scp;
        for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
            const char * name;
            assay_property_t * prp;
            name = assay_section_name_get(scp);
            printf(" [%s]\n", name);
            for (prp = assay_property_first(scp); prp != (assay_property_t *)0; prp = assay_property_next(prp)) {
                const char * key;
                const char * value;
                size_t length;
                key = assay_property_key_get(prp);
                value = (const char *)assay_property_value_get(prp, &length);
                if (diminuto_escape_printable(value)) {
                    printf("  %s=\"%s\"\n", key, value);
                } else {
                    printf("  %s=\n", key);
                    diminuto_dump_generic(stdout, value, length, 0, '.', 0, 0, 3, 1, 16, ": ", " ", "|", ' ', ' ', "|\n");
                }
            }
        }
    }
    return cfp;
}

int main(void)
{
    SETLOGMASK();

    assay_scanner_debug(!0);
    assay_parser_debug(!0);

    {
        assay_config_t * cfp;
        const char * value;
        ASSERT((cfp = import(PATH1)) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(((value = assay_config_read_string(cfp, "section1", "keyword1")) != (const char *)0) && (strcmp(value, "value1") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "section1", "keyword2")) != (const char *)0) && (strcmp(value, "value2") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "section two", "keyword3")) != (const char *)0) && (strcmp(value, "value three") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "section two", "keyword4")) != (const char *)0) && (strcmp(value, "value four") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "Section Three", "keyword5")) != (const char *)0) && (strcmp(value, "value V") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "Section Three", "keyword6")) != (const char *)0) && (strcmp(value, "value IV") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "Section Three", "keyword7")) != (const char *)0) && (strcmp(value, "\a\b\t\n\v\f\r=:[]\\!\xa\xbc\7\77\377") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "section@four.com", "Charles E. Weller")) != (const char *)0) && (strcmp(value, "Now is the time for all good men to come to the aid of their party.") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "section@four.com", "1926")) != (const char *)0) && (strcmp(value, " How now brown cow ") == 0));
        ASSERT(((value = assay_config_read_string(cfp, "section@four.com", "Lord Admiral Nelson")) != (const char *)0) && (strcmp(value, "\b England expects each man to do his duty. \b ") == 0));
        assay_config_destroy(cfp);
    }

    EXIT();
}
