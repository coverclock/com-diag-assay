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

    {
        assay_config_t * cfp;
        ASSERT((cfp = import(PATH1)) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
    }

    EXIT();
}
