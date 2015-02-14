/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_
#define _H_COM_DIAG_ASSAY_

/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * This file is part of the Digital Aggregates Corporation Assay package.
 * Assay is a C library that provides functions to parse a configuration file
 * in yet one more variation on the widely used and under specified INI
 * format. The syntax of this particular format is specified by an LALR(1)
 * grammar. Portions of the C code, specifically its lexical scanner and
 * shift-reduce parser, are generated using the Flex (Lex) and Bison (Yacc)
 * tools. Assay is built on top of the Diminuto library of GNU/Linux-based
 * software tools.
 *
 * REFERENCES
 *
 * Wikipedia, "INI file", http://en.wikipedia.org/wiki/INI_file
 *
 * Asterisk Project, "Asterisk Configuration Files",
 * https://wiki.asterisk.org/wiki/display/AST/Asterisk+Configuration+Files
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * OPAQUE TYPES
 ******************************************************************************/

struct AssayConfig;
typedef struct AssayConfig assay_config_t;

struct AssaySection;
typedef struct AssaySection assay_section_t;

struct AssayProperty;
typedef struct AssayProperty assay_property_t;

/*******************************************************************************
 * CONFIGURATION PRIMITIVES
 ******************************************************************************/

extern assay_config_t * assay_config_create(void);

extern assay_property_t * assay_config_cached(assay_config_t * cfp);

extern assay_config_t * assay_config_load(assay_config_t * cfp, FILE * fp);

extern void assay_config_destroy(assay_config_t * cfp);

/*******************************************************************************
 * SECTION PRIMITIVES
 ******************************************************************************/

extern assay_section_t * assay_section_create(assay_config_t * cfp, const char * section);

extern assay_section_t * assay_section_seek(assay_config_t * cfp, const char * section);

/* (There is deliberately no assay_section_destroy function. */

/*******************************************************************************
 * SECTION ITERATORS
 ******************************************************************************/

extern assay_section_t * assay_section_first(assay_config_t * cfp);

extern assay_section_t * assay_section_next(assay_section_t * scp);

extern assay_section_t * assay_section_prev(assay_section_t * scp);

extern assay_section_t * assay_section_last(assay_config_t * cfp);

/*******************************************************************************
 * PROPERTY PRIMITIVES
 ******************************************************************************/

extern assay_property_t * assay_property_create(assay_section_t * scp, const char * key);

extern assay_property_t * assay_property_seek(assay_section_t * scp, const char * key);

extern void assay_property_destroy(assay_property_t * prp);

/*******************************************************************************
 * PROPERTY ITERATORS
 ******************************************************************************/

extern assay_property_t * assay_property_first(assay_section_t * scp);

extern assay_property_t * assay_property_next(assay_property_t * prp);

extern assay_property_t * assay_property_prev(assay_property_t * prp);

extern assay_property_t * assay_property_last(assay_section_t * scp);

/*******************************************************************************
 * GETTORS
 ******************************************************************************/

extern const char * assay_section_get(assay_section_t * scp);

extern const char * assay_key_get(assay_property_t * prp);

extern const void * assay_value_get(assay_property_t * prp, size_t * lengthp);

/*******************************************************************************
 * SETTORS
 ******************************************************************************/

extern assay_property_t * assay_value_set(assay_property_t * prp, const void * value, size_t length);

/*******************************************************************************
 * COMPOSITES
 ******************************************************************************/

extern const void * assay_config_get_binary(assay_config_t * cfp, const char * section, const char * key, size_t * lengthp);

static inline const char * assay_config_get(assay_config_t * cfp, const char * section, const char * key)
{
    return (const char *)assay_config_get_binary(cfp, section, key, (size_t *)0);
}

extern void assay_config_set_binary(assay_config_t * cfp, const char * section, const char * key, const void * value, size_t length);

static inline void assay_config_set(assay_config_t * cfp, const char * section, const char * key, const char * value)
{
    return assay_config_set_binary(cfp, section, key, value, strlen(value) + 1);
}

/*******************************************************************************
 * AUDIT
 ******************************************************************************/

extern void * assay_config_audit(assay_config_t * cfp);

#endif
