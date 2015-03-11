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
    int rc = 0;
    assay_config_t * cfp;
    const char * value;

    diminuto_log_setmask();

    do {

        if (!(cfp = assay_config_create())) {
            perror("assay_config_create");
            rc = 2;
            break;
        }

        if (!assay_config_import_file(cfp, "-")) {
            perror("assay_config_import_file");
            rc = 3;
        } else if (argc < 2) {
            if (!assay_config_export_stream(cfp, stdout)) {
                perror("assay_config_export_stream");
                rc = 4;
            }
        } else if (argc < 3) {
            if ((value = assay_config_read_string(cfp, ASSAY_SECTION_DEFAULT, argv[1])) == (const char *)0) {
                rc = 1;
            } else {
                printf("%s\n", value);
            }
        } else {
            if ((value = assay_config_read_string(cfp, argv[1], argv[2])) == (const char *)0) {
                rc = 1;
            } else {
                 printf("%s\n", value);
            }
        }

        assay_config_destroy(cfp);

    } while (0);

   return rc;
}
