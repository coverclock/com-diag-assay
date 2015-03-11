/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * USAGE
 *
 * assay [ [ section ] key ] < file.ini
 *
 * EXAMPLES
 *
 * assay executablebinary
 *
 * ABSTRACT
 *
 * Parse the specified configuration file and extract and print to standard
 * output the value of the specified key in the specified section.
 */

#include <stdio.h>
#include <errno.h>
#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_log.h"

int main(int argc, char * argv[]) {
    assay_config_t * cfp;
    const char * value;

    diminuto_log_setmask();

    if (!(cfp = assay_config_create())) {
        perror("assay_config_create");
        return 2;
    }

    if (!assay_config_import_file(cfp, "-")) {
        perror("assay_config_import_file");
        return 3;
    }

    if (argc < 2) {
        if (!assay_config_export_stream(cfp, stdout)) {
            perror("assay_config_export_stream");
            return 4;
        }
    } else if (argc < 3) {
        value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, argv[1]);
        if (value == (const char *)0) {
            return 1;
        } else {
            printf("%s\n", value);
        }
    } else {
        value = assay_config_read_string(cfp, argv[1], argv[2]);
        if (value == (const char *)0) {
            return 1;
        } else {
            printf("%s\n", value);
        }
    }

   return 0;
}
