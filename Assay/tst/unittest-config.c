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
#include <stdio.h>
#include <string.h>

static const char PATH1[] = "etc/test1.ini";

static inline void dump(assay_config_t * cfp)
{
    assay_config_log(cfp);
}

int main(void)
{
    SETLOGMASK();

    {
        assay_config_t * cfp;
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_load_file(cfp, PATH1) == cfp);
        dump(cfp);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
    }

    EXIT();
}
