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
 *
 * VOIP Info, "Asterisk config files",
 * http://www.voip-info.org/wiki/view/Asterisk+config+files
 *
 * John R. Levine, Tony Mason, Doug Brown, "lex & yacc", 2nd ed., O'Reilly,
 * 1995
 *
 * John R. Levine, "flex & bison", O'Reilly, 2009
 *
 * Tom Niemann, "LEX & YACC TUTORIAL",
 * http://epaperpress.com/lexandyacc/download/LexAndYaccTutorial.pdf
 *
 * Saumya, K. Debray, "Lex and Yacc: A Brisk Tutorial",
 * http://www.cs.arizona.edu/~debray/Teaching/CSc453/DOCS/tutorial-large.pdf
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
 * CONSTANTS
 ******************************************************************************/

#define ASSAY_SECTION_DEFAULT_NAME "general"

extern const char ASSAY_SECTION_DEFAULT[];

/*******************************************************************************
 * CONFIGURATION PRIMITIVES
 ******************************************************************************/

/**
 * Dynamically allocate an initialize a new configuration.
 * @return a pointer to the configuration or null if an error occurred.
 */
extern assay_config_t * assay_config_create(void);

/**
 * Dynamically free an existing configuration. This is a remarkably expensive
 * operation to perform. However, most applications will never have to perform
 * it.
 * @param cfp points to a configuration.
 */
extern void assay_config_destroy(assay_config_t * cfp);

/**
 * Increment the error counter in the configuration.
 * @param cfp points to the configuration.
 * @return the current error counter.
 */
extern int assay_config_error(assay_config_t * cfp);

/*******************************************************************************
 * CONFIGURATION GETTORS
 ******************************************************************************/

/**
 * Return a pointer to a property cached in the configuration. This isn't really
 * part of the public API; it is used for unit testing.
 * @param cfp points to the configuration.
 * @return a pointer to the cached property or null if none.
 */
extern assay_property_t * assay_property_cached(assay_config_t * cfp);

/**
 * Return a pointer to a section cached in the configuration. This isn't really
 * part of the public API; it is used for unit testing.
 * @param cfp points to the configuration.
 * @return a pointer to the cached section or null if none.
 */
extern assay_section_t * assay_section_cached(assay_config_t * cfp);

/**
 * Return the error counter in the configuration. This counter is incremented
 * by the parser every time it reports a syntax error.
 * @param cfp points to the configuration.
 * @return an error count.
 */
extern int assay_config_errors(assay_config_t * cfp);

/*******************************************************************************
 * SECTION PRIMITIVES
 ******************************************************************************/

/**
 * Dynamically allocate and initialize a new section with the specified name in
 * a configuration. (There is deliberately no way to destroy a section except by
 * destroying the entire configuration.) If a section of the same name already
 * exists in the configuration, the existing section is returned. If a new
 * section is created, a copy is made of the specified name.
 * @param cfp points to the configuration.
 * @param name is the name for the new section.
 * @return a pointer to the section or null if an error occurred.
 */
extern assay_section_t * assay_section_create(assay_config_t * cfp, const char * name);

/**
 * Search for a section with the specified name in a configuration.
 * @param cfp points to the configuration.
 * @param name is the name for the new section.
 * @return a pointer to the section or null if it was not found.
 */
extern assay_section_t * assay_section_search(assay_config_t * cfp, const char * name);

/* (There is deliberately no assay_section_destroy function. */

/*******************************************************************************
 * SECTION ITERATORS
 ******************************************************************************/

/**
 * Return a pointer to the first section in the configuration.
 * @param cfp points to the configuration.
 * @return a pointer to the first section or null if none.
 */
extern assay_section_t * assay_section_first(assay_config_t * cfp);

/**
 * Return a pointer to the next section.
 * @param scp points to the prior section.
 * @return a pointer to the next section of null if none.
 */
extern assay_section_t * assay_section_next(assay_section_t * scp);

/**
 * Return a pointer to the prior section.
 * @param scp points to the next section.
 * @return a pointer to the prior section of null if none.
 */
extern assay_section_t * assay_section_prev(assay_section_t * scp);

/**
 * Return a pointer to the last section in the configuration.
 * @param cfp points to the configuration.
 * @return a pointer to the last section or null if none.
 */
extern assay_section_t * assay_section_last(assay_config_t * cfp);

/*******************************************************************************
 * SECTION GETTORS
 ******************************************************************************/

/**
 * Return the C string that is the section name.
 * @param scp points to the section.
 * @return a C string that is the section name.
 */
extern const char * assay_section_name_get(assay_section_t * scp);

/*******************************************************************************
 * PROPERTY PRIMITIVES
 ******************************************************************************/

/**
 * Dynamically allocate and initialize a new property with the specified key in
 * a section. If a property of the same key already exists in the section, the
 * existing property is returned. If a new property is created, a copy is made
 * of the specified key.
 * @param scp points to the section.
 * @param key is the name for the new property.
 * @return a pointer to the property or null if an error occurred.
 */
extern assay_property_t * assay_property_create(assay_section_t * scp, const char * key);

/**
 * Search for a property with the specified key in a section.
 * @param scp points to the section.
 * @param key is the name for the property.
 * @return a pointer to the property or null if an error occurred.
 */
extern assay_property_t * assay_property_search(assay_section_t * scp, const char * key);

/**
 * Destroy a property.
 * @param prp points to the property.
 */
extern void assay_property_destroy(assay_property_t * prp);

/*******************************************************************************
 * PROPERTY ITERATORS
 ******************************************************************************/

/**
 * Return a pointer to the first property in the section.
 * @param scp points to the configuration.
 * @return a pointer to the first property or null if none.
 */
extern assay_property_t * assay_property_first(assay_section_t * scp);

/**
 * Return a pointer to the prior property.
 * @param scp points to the next property.
 * @return a pointer to the prior property of null if none.
 */
extern assay_property_t * assay_property_next(assay_property_t * prp);

/**
 * Return a pointer to the prior property.
 * @param scp points to the next property.
 * @return a pointer to the prior property of null if none.
 */
extern assay_property_t * assay_property_prev(assay_property_t * prp);

/**
 * Return a pointer to the last property in the section.
 * @param scp points to the section.
 * @return a pointer to the last property or null if none.
 */
extern assay_property_t * assay_property_last(assay_section_t * scp);

/*******************************************************************************
 * PROPERTY GETTORS
 ******************************************************************************/

/**
 * Return the C string that is the property key.
 * @param prp points to the property.
 * @return a C string that is the property key.
 */
extern const char * assay_property_key_get(assay_property_t * prp);

/**
 * Return a pointer to the object that is the property value. The size in bytes
 * of the object is also returned if the length pointer is non-null. If the
 * value has never been set, null is returned and the length variable is not
 * modified.
 * @param prp points to the property.
 * @param lengthp points to a variable into which the object length is returned.
 * @return a pointer to the object that is the property value.
 */
extern const void * assay_property_value_get(assay_property_t * prp, size_t * lengthp);

/*******************************************************************************
 * PROPERTY SETTORS
 ******************************************************************************/

/**
 * Set the value of a property. A shallow copy is made of the value object.
 * @param prp points to the property.
 * @param value points to the value object.
 * @param length is the size of the value object.
 * @return a pointer to the property.
 */
extern assay_property_t * assay_property_value_set(assay_property_t * prp, const void * value, size_t length);

/*******************************************************************************
 * COMPOSITE OPERATORS
 ******************************************************************************/

/**
 * Return a pointer to an object and its length in bytes from a configuration
 * given a section name and a property key.
 * @param cfp points to the configuration.
 * @param name is the name of the section.
 * @param key is the key of the property.
 * @param lengthp points to a variable into which the object size is placed.
 * @return a pointer to an object or null if not found.
 */
extern const void * assay_config_read_binary(assay_config_t * cfp, const char * name, const char * key, size_t * lengthp);

/**
 * Set the value of a property and its length in bytes to a configuration
 * given a section name and a property key. If the section or property do not
 * yet exist they are each created. A shallow copy of the value object is
 * made.
 * @param cfp points to the configuration.
 * @param name is the name of the section.
 * @param key is the key of the property.
 * @param value points to the value object.
 * @param length is the size of the value object in bytes.
 */
extern void assay_config_write_binary(assay_config_t * cfp, const char * name, const char * key, const void * value, size_t length);

/*******************************************************************************
 * HELPERS
 ******************************************************************************/

/**
 * Return a C string that is a property value from a configuration given a
 * section name and a property key.
 * @param cfp points to the configuration.
 * @param name is the name of the section.
 * @param key is the key of the property.
 * @return a pointer to a C string or null if not found.
 */
static inline const char * assay_config_read_string(assay_config_t * cfp, const char * name, const char * key)
{
    return (const char *)assay_config_read_binary(cfp, name, key, (size_t *)0);
}

/**
 * Set the value of a property from a C string to a configuration given a
 * section name and a property key. If the section or property do not
 * yet exist they are each created. A shallow copy of the value C string is
 * made.
 * @param cfp points to the configuration.
 * @param name is the name of the section.
 * @param key is the key of the property.
 * @param value points to the value C string.
 */
static inline void assay_config_write_string(assay_config_t * cfp, const char * name, const char * key, const char * value)
{
    return assay_config_write_binary(cfp, name, key, value, strlen(value) + 1);
}

/*******************************************************************************
 * IMPORTERS
 ******************************************************************************/

/**
 * Import a configuration from a FILE stream.
 * @param cfp points to the configuration.
 * @param stream points to the FILE stream.
 * @return a pointer to the configuration or null if an error occurred.
 */
extern assay_config_t * assay_config_import_stream(assay_config_t * cfp, FILE * stream);

/**
 * Export a configuration to a FILE stream.
 * @param cfp points to the configuration.
 * @param stream points to the FILE stream.
 * @return a pointer to the configuration or null if an error occurred.
 */
extern assay_config_t * assay_config_export_stream(assay_config_t * cfp, FILE * stream);

/**
 * Import a configuration from a file. The file is opened for reading and closed
 * once parsed.
 * @param cfp points to the configuration.
 * @param file is the path name to the file.
 * @return a pointer to the configuration or null if an error occurred.
 */
extern assay_config_t * assay_config_import_file(assay_config_t * cfp, const char * file);

/*******************************************************************************
 * AUDITORS
 ******************************************************************************/

/**
 * Log a property at the DEBUG level.
 * @param prp points to the property.
 */
extern void assay_property_log(assay_property_t * prp);

/**
 * Log a section at the DEBUG level.
 * @param scp points to the section.
 */
extern void assay_section_log(assay_section_t * scp);

/**
 * Log a configuration at the DEBUG level.
 * @param cfp points to the configuration.
 */
extern void assay_config_log(assay_config_t * cfp);

/**
 * Audit a configuration.
 * @param cfp points to the configuration.
 * @return a pointer to the first incorrect object (whatever it is) or null.
 */
extern void * assay_config_audit(assay_config_t * cfp);

#endif
