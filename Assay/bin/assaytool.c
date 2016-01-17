/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015-2016 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * EXAMPLES
 *
 * assaytool -k LEFT -v RIGHT -w -f test.conf -e
 * assaytool -f test.conf -i -k LEFT -r
 * assaytool -f test.conf -i -k FOO -v BAR -w -e
 * assaytool -i -k FOO -r < test.conf
 * assaytool -f test.conf -i -f - -e
 *
 * USAGE
 *
 * assaytool [ -d | -f FILE | -i | -s SECTION | -S | -k KEYWORD | -v VALUE | -r | | -w | -x | -e | ... ]
 *
 * ABSTRACT
 *
 * Implements a general command line tool to manipulate configuration files
 * in the Assay format. Command line parameters are interpreted and executed
 * as they are encountered left to right in a memtool-like manner.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_log.h"

static const char * program = "assaytool";

static void usage(void)
{
    fprintf(stderr, "usage: %s [ -d ] [ -f FILE | -i | -e | -s SECTION | -S | -k KEYWORD | -v VALUE | -r | -w | -x | ... ]\n", program);
    fprintf(stderr, "       -d            Enable debug mode\n");
    fprintf(stderr, "       -f FILE       Use file FILE\n");
    fprintf(stderr, "       -i            import FILE\n");
    fprintf(stderr, "       -e            export FILE\n");
    fprintf(stderr, "       -s SECTION    Use section SECTION\n");
    fprintf(stderr, "       -S            Use section default\n");
    fprintf(stderr, "       -k KEYWORD    Use keyword KEYWORD\n");
    fprintf(stderr, "       -v VALUE      Use value VALUE\n");
    fprintf(stderr, "       -r            Read property KEYWORD in SECTION\n");
    fprintf(stderr, "       -w            Write VALUE to property KEYWORD in SECTION\n");
    fprintf(stderr, "       -x            Destroy property KEYWORD in SECTION\n");
    fprintf(stderr, "       -?            Print menu\n");
}

int main(int argc, char * argv[])
{
    int rc = 0;
    assay_config_t * cfp = (assay_config_t *)0;
    const char * path = "-";
    const char * section = ASSAY_SECTION_DEFAULT;
    const char * keyword = "";
    char * value = "";
    int debug = 0;
    FILE * fp = (FILE *)0;
    int opt;
    extern char * optarg;

    do {
    
        program = strrchr(argv[0], '/');
        program = (program == (char *)0) ? argv[0] : program + 1;
    
        if (!(cfp = assay_config_create())) {
            perror("assay_config_create");
            rc = 3;
            break;
        }
    
        while ((opt = getopt(argc, argv, "df:s:Sk:v:ierwx?")) >= 0) {
    
            switch (opt) {
    
            case 'd':
                debug = !0;
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                diminuto_log_setmask();
                break;
    
            case 'f':
                path = optarg;
                if (debug) { fprintf(stderr, "%s -%c \"%s\"\n", program, opt, path); }
                break;
    
            case 's':
                section = optarg;
                if (debug) { fprintf(stderr, "%s -%c \"%s\"\n", program, opt, section); }
                break;
    
            case 'S':
                section = ASSAY_SECTION_DEFAULT;
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                break;
    
            case 'k':
                keyword = optarg;
                if (debug) { fprintf(stderr, "%s -%c \"%s\"\n", program, opt, keyword); }
                break;
    
            case 'v':
                value = optarg;
                if (debug) { fprintf(stderr, "%s -%c \"%s\"\n", program, opt, value); }
                break;
    
            case 'i':
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                if (!assay_config_import_file(cfp, path)) {
                    perror("assay_config_import_file");
                    rc = 2;
                }
                break;
    
            case 'e':
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                if (!assay_config_export_file(cfp, path)) {
                    perror("assay_config_export_file");
                    rc = 2;
                }
                break;
    
            case 'r':
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                if ((value = assay_config_read_string(cfp, section, keyword)) != (const char *)0) {
                    printf("%s\n", value);
                }
                break;
    
            case 'w':
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                assay_config_write_string(cfp, section, keyword, value);
                break;
    
            case 'x':
                if (debug) { fprintf(stderr, "%s -%c\n", program, opt); }
                assay_config_destroy_string(cfp, section, keyword);
                break;
    
            case '?':
                usage();
                break;
    
            default:
                usage();
                rc = 1;
    
            }
    
            if (rc != 0) {
                break;
            }
    
        }
    
        assay_config_destroy(cfp);

    } while (false);

    exit(rc);
}
