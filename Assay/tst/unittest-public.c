/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Diminuto.html<BR>
 *
 * Best if run under valgrind(1)!
 */

#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void dump(assay_config_t * cfp)
{
    assay_section_t * scp;
    assay_property_t * prp;
    const char * key;
    const char * value;
    size_t length;
    size_t ii;
    int printable;

    if (cfp == (assay_config_t *)0) {
        DIMINUTO_LOG_DEBUG("assay@%p\n", cfp);
    } else {
        DIMINUTO_LOG_DEBUG("assay@%p:\n", cfp);
        for (scp = assay_section_first(cfp); scp != (assay_section_t *)0; scp = assay_section_next(scp)) {
            DIMINUTO_LOG_DEBUG("section@%p: [%s]\n", scp, assay_section_get(scp));
            for (prp = assay_property_first(scp); prp != (assay_property_t *)0; prp = assay_property_next(prp)) {
                printable = !0;
                key = assay_key_get(prp);
                value = assay_value_get(prp, &length);
                if (value == (void *)0) {
                    DIMINUTO_LOG_DEBUG("property@%p: %s=%p\n", prp, key, value);
                } else {
                    for (ii = 0; ii < length - 1; ++ii) {
                        if (!isprint(value[ii])) {
                            printable = 0;
                            break;
                        }
                    }
                    if (printable) {
                        DIMINUTO_LOG_DEBUG("property@%p: %s=\"%s\"[%zu]\n", prp, key, value, length);
                    } else {
                        DIMINUTO_LOG_DEBUG("property@%p: %s=<\n", prp, key);
                        diminuto_dump(stderr, value, length);
                        DIMINUTO_LOG_DEBUG(">[%zu]\n", length);
                    }
                }
            }
        }
    }
}

int main(void)
{
    SETLOGMASK();

    {
        assay_config_t * cfp;
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
    }

    {
        assay_config_t * cfp;
        assay_section_t * scp;
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, "Alfa") == (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT((scp = assay_section_create(cfp, "Alfa")) != (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_section_search(cfp, "Alfa") == scp);
        ASSERT(assay_section_create(cfp, "Alfa") == scp);
        ASSERT(assay_section_first(cfp) == scp);
        ASSERT(assay_section_next(scp) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == scp);
        ASSERT(assay_property_search(scp, "One") == (assay_property_t *)0);
        ASSERT(assay_property_first(scp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == (assay_property_t *)0);
        assay_config_destroy(cfp);
    }

    {
        assay_config_t * cfp;
        assay_section_t * scp[3];
        assay_property_t * prp;
        const char * value;
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, "Alfa") == (assay_section_t *)0);
        ASSERT(assay_section_search(cfp, "Bravo") == (assay_section_t *)0);
        ASSERT(assay_section_search(cfp, "Charlie") == (assay_section_t *)0);
        ASSERT((scp[0] = assay_section_create(cfp, "Alfa")) != (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, "Alfa") == scp[0]);
        ASSERT(assay_section_search(cfp, "Bravo") == (assay_section_t *)0);
        ASSERT(assay_section_search(cfp, "Charlie") == (assay_section_t *)0);
        ASSERT(assay_section_first(cfp) == scp[0]);
        ASSERT(assay_section_next(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == scp[0]);
        ASSERT((scp[1] = assay_section_create(cfp, "Bravo")) != (assay_section_t *)0);
        ASSERT(scp[1] != scp[0]);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, "Alfa") == scp[0]);
        ASSERT(assay_section_search(cfp, "Bravo") == scp[1]);
        ASSERT(assay_section_search(cfp, "Charlie") == (assay_section_t *)0);
        ASSERT(assay_section_first(cfp) == scp[0]);
        ASSERT(assay_section_next(scp[0]) == scp[1]);
        ASSERT(assay_section_prev(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_next(scp[1]) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp[1]) == scp[0]);
        ASSERT(assay_section_last(cfp) == scp[1]);
        ASSERT((scp[2] = assay_section_create(cfp, "Charlie")) != (assay_section_t *)0);
        ASSERT(scp[2] != scp[0]);
        ASSERT(scp[2] != scp[1]);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, "Alfa") == scp[0]);
        ASSERT(assay_section_search(cfp, "Bravo") == scp[1]);
        ASSERT(assay_section_search(cfp, "Charlie") == scp[2]);
        ASSERT(assay_section_first(cfp) == scp[0]);
        ASSERT(assay_section_next(scp[0]) == scp[1]);
        ASSERT(assay_section_prev(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_next(scp[1]) == scp[2]);
        ASSERT(assay_section_prev(scp[1]) == scp[0]);
        ASSERT(assay_section_next(scp[2]) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp[2]) == scp[1]);
        ASSERT(assay_section_last(cfp) == scp[2]);
        assay_config_destroy(cfp);
    }

    {
        assay_config_t * cfp;
        assay_section_t * scp;
        assay_property_t * prp;
        size_t length;
        static const char SECTION[] = "Alfa";
        static const char KEY[] = "One";
        static const char VALUE[] = "1";
        /**/
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        /**/
        ASSERT(assay_section_search(cfp, SECTION) == (assay_section_t *)0);
        ASSERT(assay_section_first(cfp) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == (assay_section_t *)0);
        ASSERT((scp = assay_section_create(cfp, SECTION)) != (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_section_search(cfp, SECTION) == scp);
        ASSERT(assay_section_first(cfp) == scp);
        ASSERT(assay_section_next(scp) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == scp);
        ASSERT(assay_section_get(scp) != (const char *)0);
        ASSERT(assay_section_get(scp) != SECTION);
        ASSERT(strcmp(assay_section_get(scp), SECTION) == 0);
        /**/
        ASSERT(assay_property_search(scp, KEY) == (assay_property_t *)0);
        ASSERT(assay_property_first(scp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == (assay_property_t *)0);
        ASSERT((prp = assay_property_create(scp, KEY)) != (assay_property_t *)0);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_property_search(scp, KEY) == prp);
        ASSERT(assay_property_first(scp) == prp);
        ASSERT(assay_property_next(prp) == (assay_property_t *)0);
        ASSERT(assay_property_prev(prp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == prp);
        ASSERT(assay_key_get(prp) != (const char *)0);
        ASSERT(assay_key_get(prp) != KEY);
        ASSERT(strcmp(assay_key_get(prp), KEY) == 0);
        /**/
        length = 0xa5a5a5a5;
        ASSERT(assay_value_get(prp, &length) == (void *)0);
        ASSERT(length == 0xa5a5a5a5);
        ASSERT(assay_value_set(prp, VALUE, sizeof(VALUE)) == prp);
        ASSERT(assay_value_get(prp, &length) != (void *)0);
        ASSERT(length == sizeof(VALUE));
        ASSERT((const char *)assay_value_get(prp, &length) != VALUE);
        ASSERT(strcmp((const char *)assay_value_get(prp, &length), VALUE) == 0);
        /**/
        assay_property_destroy(prp);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_property_search(scp, KEY) == (assay_property_t *)0);
        ASSERT(assay_property_first(scp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == (assay_property_t *)0);
        /**/
        assay_config_destroy(cfp);
    }

    {
        assay_config_t * cfp;
        {
            ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp[3];
            assay_property_t * prp[6];
            size_t length;
            ASSERT((scp[0] = assay_section_create(cfp, "Alfa")) != (assay_section_t *)0);
            ASSERT((prp[0] = assay_property_create(scp[0], "One")) != (assay_property_t *)0);
            ASSERT(assay_value_set(prp[0], "1", strlen("1") + 1) == prp[0]);
            ASSERT((scp[1] = assay_section_create(cfp, "Bravo")) != (assay_section_t *)0);
            ASSERT((prp[1] = assay_property_create(scp[1], "One")) != (assay_property_t *)0);
            ASSERT((prp[2] = assay_property_create(scp[1], "Two")) != (assay_property_t *)0);
            ASSERT(assay_value_set(prp[1], "11", strlen("11") + 1) == prp[1]);
            ASSERT(assay_value_set(prp[2], "22", strlen("22") + 1) == prp[2]);
            ASSERT((scp[2] = assay_section_create(cfp, "Charlie")) != (assay_section_t *)0);
            ASSERT((prp[3] = assay_property_create(scp[2], "One")) != (assay_property_t *)0);
            ASSERT((prp[4] = assay_property_create(scp[2], "Two")) != (assay_property_t *)0);
            ASSERT((prp[5] = assay_property_create(scp[2], "Three")) != (assay_property_t *)0);
            ASSERT(assay_value_set(prp[3], "111", strlen("111") + 1) == prp[3]);
            ASSERT(assay_value_set(prp[4], "222", strlen("222") + 1) == prp[4]);
            ASSERT(assay_value_set(prp[5], "333", strlen("113") + 1) == prp[5]);
            /**/
            ASSERT(assay_property_first(scp[0]) == prp[0]);
            ASSERT(assay_property_next(prp[0]) == (assay_property_t *)0);
            ASSERT(assay_property_prev(prp[0]) == (assay_property_t *)0);
            ASSERT(assay_property_last(scp[0]) == prp[0]);
            /**/
            ASSERT(assay_property_first(scp[1]) == prp[1]);
            ASSERT(assay_property_next(prp[1]) == prp[2]);
            ASSERT(assay_property_prev(prp[1]) == (assay_property_t *)0);
            ASSERT(assay_property_next(prp[2]) == (assay_property_t *)0);
            ASSERT(assay_property_prev(prp[2]) == prp[1]);
            ASSERT(assay_property_last(scp[1]) == prp[2]);
            /**/
            ASSERT(assay_property_first(scp[2]) == prp[3]);
            ASSERT(assay_property_next(prp[3]) == prp[5]);
            ASSERT(assay_property_prev(prp[3]) == (assay_property_t *)0);
            ASSERT(assay_property_next(prp[5]) == prp[4]);
            ASSERT(assay_property_prev(prp[5]) == prp[3]);
            ASSERT(assay_property_next(prp[4]) == (assay_property_t *)0);
            ASSERT(assay_property_prev(prp[4]) == prp[5]);
            ASSERT(assay_property_last(scp[2]) == prp[4]);
        }
        dump(cfp);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp;
            assay_property_t * prp;
            const char * key;
            const char * value;
            size_t length;
            /**/
            ASSERT((scp = assay_section_search(cfp, "Alfa")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "One") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT(assay_value_set(prp, "AAA", strlen("AAA") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, "Two")) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, "Three")) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, "Bravo")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "One") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT(assay_value_set(prp, "AA", strlen("AA") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, "Two")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "Two") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT(assay_value_set(prp, "BB", strlen("BB") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, "Three")) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, "Charlie")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "One") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT(assay_value_set(prp, "A", strlen("A") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, "Two")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "Two") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT(assay_value_set(prp, "B", strlen("B") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, "Three")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "Three") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "333") == 0);
            ASSERT(assay_value_set(prp, "C", strlen("C") + 1) == prp);
        }
        dump(cfp);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp;
            assay_property_t * prp;
            const char * key;
            const char * value;
            size_t length;
            /**/
            ASSERT((scp = assay_section_search(cfp, "Alfa")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "One") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((prp = assay_property_search(scp, "Two")) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, "Three")) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, "Bravo")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "One") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((prp = assay_property_search(scp, "Two")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "Two") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((prp = assay_property_search(scp, "Three")) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, "Charlie")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "One") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((prp = assay_property_search(scp, "Two")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "Two") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "B") == 0);
            ASSERT((prp = assay_property_search(scp, "Three")) != (assay_property_t *)0);
            ASSERT((key = assay_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, "Three") == 0);
            ASSERT((value = (const char *)assay_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "C") == 0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp;
            assay_property_t * prp;
            /**/
            ASSERT((scp = assay_section_search(cfp, "Alfa")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, "One") == (assay_property_t *)0);
            ASSERT(assay_section_search(cfp, "Alfa") == scp);
            /**/
            ASSERT((scp = assay_section_search(cfp, "Bravo")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, "One") == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, "Two")) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, "Two") == (assay_property_t *)0);
            ASSERT(assay_section_search(cfp, "Bravo") == scp);
            /**/
            ASSERT((scp = assay_section_search(cfp, "Charlie")) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, "One")) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, "One") == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, "Two")) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, "Two") == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, "Three")) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, "Three") == (assay_property_t *)0);
            ASSERT(assay_section_search(cfp, "Charlie") == scp);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
    }

    {
        assay_config_t * cfp;
        const char * value;
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) == (const char *)0);
            /**/
            assay_config_set_string(cfp, "Alfa", "One", "1");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) == (const char *)0);
            /**/
            assay_config_set_string(cfp, "Bravo", "One", "11");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) == (const char *)0);
            assay_config_set_string(cfp, "Bravo", "Two", "22");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) == (const char *)0);
            /**/
            assay_config_set_string(cfp, "Charlie", "One", "111");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) == (const char *)0);
            assay_config_set_string(cfp, "Charlie", "Two", "222");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) == (const char *)0);
            assay_config_set_string(cfp, "Charlie", "Three", "333");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            /**/
            assay_config_set_string(cfp, "Alfa", "One", "A");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            /**/
            assay_config_set_string(cfp, "Bravo", "One", "AA");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            assay_config_set_string(cfp, "Bravo", "Two", "BB");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            /**/
            assay_config_set_string(cfp, "Charlie", "One", "AAA");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            assay_config_set_string(cfp, "Charlie", "Two", "BBB");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "BBB") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            assay_config_set_string(cfp, "Charlie", "Three", "CCC");
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "One")) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Two")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Alfa", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Bravo", "Three")) == (const char *)0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "One")) != (const char *)0);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Two")) != (const char *)0);
            ASSERT(strcmp(value, "BBB") == 0);
            ASSERT((value = assay_config_get_string(cfp, "Charlie", "Three")) != (const char *)0);
            ASSERT(strcmp(value, "CCC") == 0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
   }

    {
        assay_config_t * cfp;
        {
            ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
            ASSERT(assay_config_cached(cfp) == (assay_property_t *)0);
        }
        ASSERT(assay_config_audit(cfp) == 0);
        {
            assay_config_set_string(cfp, "Alfa", "One", "1");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Alfa"), "One"));
            assay_config_set_string(cfp, "Bravo", "One", "11");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Bravo"), "One"));
            assay_config_set_string(cfp, "Bravo", "Two", "22");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Bravo"), "Two"));
            assay_config_set_string(cfp, "Charlie", "One", "111");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "One"));
            assay_config_set_string(cfp, "Charlie", "Two", "222");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "Two"));
            assay_config_set_string(cfp, "Charlie", "Three", "333");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "Three"));
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_config_get_string(cfp, "Alfa", "One");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Alfa"), "One"));
            assay_config_get_string(cfp, "Bravo", "One");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Bravo"), "One"));
            assay_config_get_string(cfp, "Bravo", "Two");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Bravo"), "Two"));
            assay_config_get_string(cfp, "Charlie", "One");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "One"));
            assay_config_get_string(cfp, "Charlie", "Two");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "Two"));
            assay_config_get_string(cfp, "Charlie", "Three");
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "Three"));
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            ASSERT(assay_config_cached(cfp) == assay_property_search(assay_section_search(cfp, "Charlie"), "Three"));
            assay_property_destroy(assay_property_search(assay_section_search(cfp, "Charlie"), "Three"));
            ASSERT(assay_property_search(assay_section_search(cfp, "Charlie"), "Three") == (assay_property_t *)0);
            ASSERT(assay_config_cached(cfp) == (assay_property_t *)0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
    }

    EXIT();
}
