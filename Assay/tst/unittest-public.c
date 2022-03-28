/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Diminuto.html<BR>
 *
 * This tests the Assay public API independent of the Flex and Bison components.
 *
 * Best if run under valgrind(1).
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "com/diag/assay/assay.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_dump.h"

static const char ALFA[] = "Alfa";
static const char BRAVO[] = "Bravo";
static const char CHARLIE[] = "Charlie";

static const char ONE[] = "One";
static const char TWO[] = "Two";
static const char THREE[] = "Three";

int main(void)
{
    SETLOGMASK();

    {
        assay_config_t * cfp;
        TEST();
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        assay_config_t * cfp;
        assay_section_t * scp;
        TEST();
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, ALFA) == (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT((scp = assay_section_create(cfp, ALFA)) != (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_section_search(cfp, ALFA) == scp);
        ASSERT(assay_section_create(cfp, ALFA) == scp);
        ASSERT(assay_section_first(cfp) == scp);
        ASSERT(assay_section_next(scp) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == scp);
        ASSERT(assay_property_search(scp, ONE) == (assay_property_t *)0);
        ASSERT(assay_property_first(scp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == (assay_property_t *)0);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        assay_config_t * cfp;
        assay_section_t * scp[3];
        assay_property_t * prp;
        const char * value;
        TEST();
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, ALFA) == (assay_section_t *)0);
        ASSERT(assay_section_search(cfp, BRAVO) == (assay_section_t *)0);
        ASSERT(assay_section_search(cfp, CHARLIE) == (assay_section_t *)0);
        ASSERT((scp[0] = assay_section_create(cfp, ALFA)) != (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, ALFA) == scp[0]);
        ASSERT(assay_section_search(cfp, BRAVO) == (assay_section_t *)0);
        ASSERT(assay_section_search(cfp, CHARLIE) == (assay_section_t *)0);
        ASSERT(assay_section_first(cfp) == scp[0]);
        ASSERT(assay_section_next(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == scp[0]);
        ASSERT((scp[1] = assay_section_create(cfp, BRAVO)) != (assay_section_t *)0);
        ASSERT(scp[1] != scp[0]);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, ALFA) == scp[0]);
        ASSERT(assay_section_search(cfp, BRAVO) == scp[1]);
        ASSERT(assay_section_search(cfp, CHARLIE) == (assay_section_t *)0);
        ASSERT(assay_section_first(cfp) == scp[0]);
        ASSERT(assay_section_next(scp[0]) == scp[1]);
        ASSERT(assay_section_prev(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_next(scp[1]) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp[1]) == scp[0]);
        ASSERT(assay_section_last(cfp) == scp[1]);
        ASSERT((scp[2] = assay_section_create(cfp, CHARLIE)) != (assay_section_t *)0);
        ASSERT(scp[2] != scp[0]);
        ASSERT(scp[2] != scp[1]);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT(assay_section_search(cfp, ALFA) == scp[0]);
        ASSERT(assay_section_search(cfp, BRAVO) == scp[1]);
        ASSERT(assay_section_search(cfp, CHARLIE) == scp[2]);
        ASSERT(assay_section_first(cfp) == scp[0]);
        ASSERT(assay_section_next(scp[0]) == scp[1]);
        ASSERT(assay_section_prev(scp[0]) == (assay_section_t *)0);
        ASSERT(assay_section_next(scp[1]) == scp[2]);
        ASSERT(assay_section_prev(scp[1]) == scp[0]);
        ASSERT(assay_section_next(scp[2]) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp[2]) == scp[1]);
        ASSERT(assay_section_last(cfp) == scp[2]);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        assay_config_t * cfp;
        assay_section_t * scp;
        assay_property_t * prp;
        size_t length;
        static const char VALUE[] = "1";
        TEST();
        /**/
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        /**/
        ASSERT(assay_section_search(cfp, ALFA) == (assay_section_t *)0);
        ASSERT(assay_section_first(cfp) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == (assay_section_t *)0);
        ASSERT((scp = assay_section_create(cfp, ALFA)) != (assay_section_t *)0);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_section_search(cfp, ALFA) == scp);
        ASSERT(assay_section_first(cfp) == scp);
        ASSERT(assay_section_next(scp) == (assay_section_t *)0);
        ASSERT(assay_section_prev(scp) == (assay_section_t *)0);
        ASSERT(assay_section_last(cfp) == scp);
        ASSERT(assay_section_name_get(scp) != (const char *)0);
        ASSERT(assay_section_name_get(scp) != ALFA);
        ASSERT(strcmp(assay_section_name_get(scp), ALFA) == 0);
        /**/
        ASSERT(assay_property_search(scp, ONE) == (assay_property_t *)0);
        ASSERT(assay_property_first(scp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == (assay_property_t *)0);
        ASSERT((prp = assay_property_create(scp, ONE)) != (assay_property_t *)0);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_property_search(scp, ONE) == prp);
        ASSERT(assay_property_first(scp) == prp);
        ASSERT(assay_property_next(prp) == (assay_property_t *)0);
        ASSERT(assay_property_prev(prp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == prp);
        ASSERT(assay_property_key_get(prp) != (const char *)0);
        ASSERT(assay_property_key_get(prp) != ONE);
        ASSERT(strcmp(assay_property_key_get(prp), ONE) == 0);
        /**/
        length = 0xa5a5a5a5;
        ASSERT(assay_property_value_get(prp, &length) == (void *)0);
        ASSERT(length == 0xa5a5a5a5);
        ASSERT(assay_property_value_set(prp, VALUE, sizeof(VALUE)) == prp);
        ASSERT(assay_property_value_get(prp, &length) != (void *)0);
        ASSERT(length == sizeof(VALUE));
        ASSERT((const char *)assay_property_value_get(prp, &length) != VALUE);
        ASSERT(strcmp((const char *)assay_property_value_get(prp, &length), VALUE) == 0);
        /**/
        assay_property_destroy(prp);
        ASSERT(assay_config_audit(cfp) == 0);
        ASSERT(assay_property_search(scp, ONE) == (assay_property_t *)0);
        ASSERT(assay_property_first(scp) == (assay_property_t *)0);
        ASSERT(assay_property_last(scp) == (assay_property_t *)0);
        /**/
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        assay_config_t * cfp;
        TEST();
        {
            ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp[3];
            assay_property_t * prp[6];
            size_t length;
            ASSERT((scp[0] = assay_section_create(cfp, ALFA)) != (assay_section_t *)0);
            ASSERT((prp[0] = assay_property_create(scp[0], ONE)) != (assay_property_t *)0);
            ASSERT(assay_property_value_set(prp[0], "1", strlen("1") + 1) == prp[0]);
            ASSERT((scp[1] = assay_section_create(cfp, BRAVO)) != (assay_section_t *)0);
            ASSERT((prp[1] = assay_property_create(scp[1], ONE)) != (assay_property_t *)0);
            ASSERT((prp[2] = assay_property_create(scp[1], TWO)) != (assay_property_t *)0);
            ASSERT(assay_property_value_set(prp[1], "11", strlen("11") + 1) == prp[1]);
            ASSERT(assay_property_value_set(prp[2], "22", strlen("22") + 1) == prp[2]);
            ASSERT((scp[2] = assay_section_create(cfp, CHARLIE)) != (assay_section_t *)0);
            ASSERT((prp[3] = assay_property_create(scp[2], ONE)) != (assay_property_t *)0);
            ASSERT((prp[4] = assay_property_create(scp[2], TWO)) != (assay_property_t *)0);
            ASSERT((prp[5] = assay_property_create(scp[2], THREE)) != (assay_property_t *)0);
            ASSERT(assay_property_value_set(prp[3], "111", strlen("111") + 1) == prp[3]);
            ASSERT(assay_property_value_set(prp[4], "222", strlen("222") + 1) == prp[4]);
            ASSERT(assay_property_value_set(prp[5], "333", strlen("113") + 1) == prp[5]);
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
        assay_config_log(cfp);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp;
            assay_property_t * prp;
            const char * key;
            const char * value;
            size_t length;
            /**/
            ASSERT((scp = assay_section_search(cfp, ALFA)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, ONE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT(assay_property_value_set(prp, "AAA", strlen("AAA") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, TWO)) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, THREE)) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, BRAVO)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, ONE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT(assay_property_value_set(prp, "AA", strlen("AA") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, TWO)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, TWO) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT(assay_property_value_set(prp, "BB", strlen("BB") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, THREE)) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, CHARLIE)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, ONE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT(assay_property_value_set(prp, "A", strlen("A") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, TWO)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, TWO) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT(assay_property_value_set(prp, "B", strlen("B") + 1) == prp);
            ASSERT((prp = assay_property_search(scp, THREE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, THREE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "333") == 0);
            ASSERT(assay_property_value_set(prp, "C", strlen("C") + 1) == prp);
        }
        assay_config_log(cfp);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp;
            assay_property_t * prp;
            const char * key;
            const char * value;
            size_t length;
            /**/
            ASSERT((scp = assay_section_search(cfp, ALFA)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, ONE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 4);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((prp = assay_property_search(scp, TWO)) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, THREE)) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, BRAVO)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, ONE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((prp = assay_property_search(scp, TWO)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, TWO) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 3);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((prp = assay_property_search(scp, THREE)) == (assay_property_t *)0);
            /**/
            ASSERT((scp = assay_section_search(cfp, CHARLIE)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, ONE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((prp = assay_property_search(scp, TWO)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, TWO) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "B") == 0);
            ASSERT((prp = assay_property_search(scp, THREE)) != (assay_property_t *)0);
            ASSERT((key = assay_property_key_get(prp)) != (const char *)0);
            ASSERT(strcmp(key, THREE) == 0);
            ASSERT((value = (const char *)assay_property_value_get(prp, &length)) != (const char *)0);
            ASSERT(length == 2);
            ASSERT(strcmp(value, "C") == 0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_section_t * scp;
            assay_property_t * prp;
            /**/
            ASSERT((scp = assay_section_search(cfp, ALFA)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, ONE) == (assay_property_t *)0);
            ASSERT(assay_section_search(cfp, ALFA) == scp);
            /**/
            ASSERT((scp = assay_section_search(cfp, BRAVO)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, ONE) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, TWO)) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, TWO) == (assay_property_t *)0);
            ASSERT(assay_section_search(cfp, BRAVO) == scp);
            /**/
            ASSERT((scp = assay_section_search(cfp, CHARLIE)) != (assay_section_t *)0);
            ASSERT((prp = assay_property_search(scp, ONE)) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, ONE) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, TWO)) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, TWO) == (assay_property_t *)0);
            ASSERT((prp = assay_property_search(scp, THREE)) != (assay_property_t *)0);
            assay_property_destroy(prp);
            ASSERT(assay_property_search(scp, THREE) == (assay_property_t *)0);
            ASSERT(assay_section_search(cfp, CHARLIE) == scp);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        assay_config_t * cfp;
        const char * value;
        TEST();
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) == (const char *)0);
            /**/
            assay_config_write_string(cfp, ALFA, ONE, "1");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) == (const char *)0);
            /**/
            assay_config_write_string(cfp, BRAVO, ONE, "11");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            assay_config_log(cfp);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) == (const char *)0);
            assay_config_write_string(cfp, BRAVO, TWO, "22");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) == (const char *)0);
            /**/
            assay_config_write_string(cfp, CHARLIE, ONE, "111");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) == (const char *)0);
            assay_config_write_string(cfp, CHARLIE, TWO, "222");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) == (const char *)0);
            assay_config_write_string(cfp, CHARLIE, THREE, "333");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "1") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            /**/
            assay_config_write_string(cfp, ALFA, ONE, "A");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "11") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            /**/
            assay_config_write_string(cfp, BRAVO, ONE, "AA");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "22") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            assay_config_write_string(cfp, BRAVO, TWO, "BB");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "111") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            /**/
            assay_config_write_string(cfp, CHARLIE, ONE, "AAA");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "222") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            assay_config_write_string(cfp, CHARLIE, TWO, "BBB");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "BBB") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "333") == 0);
            assay_config_write_string(cfp, CHARLIE, THREE, "CCC");
            ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "A") == 0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, TWO)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, ALFA, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AA") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "BB") == 0);
            ASSERT((value = assay_config_read_string(cfp, BRAVO, THREE)) == (const char *)0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, ONE)) != (const char *)0);
            ASSERT(strcmp(value, "AAA") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, TWO)) != (const char *)0);
            ASSERT(strcmp(value, "BBB") == 0);
            ASSERT((value = assay_config_read_string(cfp, CHARLIE, THREE)) != (const char *)0);
            ASSERT(strcmp(value, "CCC") == 0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
        STATUS();
   }

    {
        assay_config_t * cfp;
        TEST();
        {
            ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
            ASSERT(assay_section_cached(cfp) == (assay_section_t *)0);
            ASSERT(assay_property_cached(cfp) == (assay_property_t *)0);
        }
        ASSERT(assay_config_audit(cfp) == 0);
        {
            assay_config_write_string(cfp, ALFA, ONE, "1");
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, ALFA));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, ALFA), ONE));
            assay_config_write_string(cfp, BRAVO, ONE, "11");
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, BRAVO));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, BRAVO), ONE));
            assay_config_write_string(cfp, BRAVO, TWO, "22");
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, BRAVO));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, BRAVO), TWO));
            assay_config_write_string(cfp, CHARLIE, ONE, "111");
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), ONE));
            assay_config_write_string(cfp, CHARLIE, TWO, "222");
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), TWO));
            assay_config_write_string(cfp, CHARLIE, THREE, "333");
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), THREE));
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            assay_config_read_string(cfp, ALFA, ONE);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, ALFA));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, ALFA), ONE));
            assay_config_read_string(cfp, BRAVO, ONE);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, BRAVO));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, BRAVO), ONE));
            assay_config_read_string(cfp, BRAVO, TWO);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, BRAVO));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, BRAVO), TWO));
            assay_config_read_string(cfp, CHARLIE, ONE);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), ONE));
            assay_config_read_string(cfp, CHARLIE, TWO);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), TWO));
            assay_config_read_string(cfp, CHARLIE, THREE);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), THREE));
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        {
            ASSERT(assay_section_cached(cfp) != (assay_section_t *)0);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_search(assay_section_search(cfp, CHARLIE), THREE) != (assay_property_t *)0);
            ASSERT(assay_property_cached(cfp) == assay_property_search(assay_section_search(cfp, CHARLIE), THREE));
            assay_property_destroy(assay_property_search(assay_section_search(cfp, CHARLIE), THREE));
            ASSERT(assay_section_search(cfp, CHARLIE) != (assay_section_t *)0);
            ASSERT(assay_section_cached(cfp) == assay_section_search(cfp, CHARLIE));
            ASSERT(assay_property_search(assay_section_search(cfp, CHARLIE), THREE) == (assay_property_t *)0);
            ASSERT(assay_property_cached(cfp) == (assay_property_t *)0);
        }
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
        STATUS();
    }

    {
        TEST();
        ASSERT(strcmp(ASSAY_CHARACTERS_SPECIAL, ASSAY_CHARACTERS_SPECIAL_STRING) == 0);
        ASSERT(strcmp(ASSAY_CHARACTERS_EXTRASPECIAL, ASSAY_CHARACTERS_EXTRASPECIAL_STRING) == 0);
        ASSERT(strcmp(ASSAY_SECTION_DEFAULT, ASSAY_SECTION_DEFAULT_STRING) == 0);
        ASSERT(strcmp(ASSAY_END_OF_TRANSMISSION, ASSAY_END_OF_TRANSMISSION_STRING) == 0);
        STATUS();
    }

    {
        assay_config_t * cfp;
        char * value;
        TEST();
        ASSERT((cfp = assay_config_create()) != (assay_config_t *)0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) == (const char *)0);
        assay_config_write_string(cfp, ALFA, ONE, "1");
        ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
        ASSERT(strcmp(value, "1") == 0);
        *value = '2';
        ASSERT((value = assay_config_read_string(cfp, ALFA, ONE)) != (const char *)0);
        ASSERT(strcmp(value, "2") == 0);
        ASSERT(assay_config_audit(cfp) == (void *)0);
        assay_config_destroy(cfp);
        STATUS();
    }

    EXIT();
}
