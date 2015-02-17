/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_PARSER_ANNEX_
#define _H_COM_DIAG_ASSAY_PARSER_ANNEX_

/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include "com/diag/assay/assay.h"

static const char ASSAY_PARSER_SECTION_DEFAULT[] = "general";

/*******************************************************************************
 * CONFIGURATION
 ******************************************************************************/

/**
 * Enable or disable debugging in the parser and return the prior value.
 * @param enable is true to enable, false to disable.
 * @return the prior value.
 */
extern int assay_parser_debug(int enable);

/**
 * Set the output configuration for the parser and return the prior value.
 * @param cfp points to the new output parser configuration.
 * @return the pointer to the prior output parser configuration.
 */
extern assay_config_t * assay_parser_output(assay_config_t * cfp);

extern const char * assay_parser_file(const char * path);

extern int assay_parser_line(int origin);

extern void assay_parser_next(void);

extern void assay_parser_error(const char * msg);

/*******************************************************************************
 * SECTION
 ******************************************************************************/

/**
 * Indicate the beginning of a section name in the parsed stream of input
 * tokens.
 */
extern void assay_parser_section_begin(void);

/**
 * Append a character to the section name being assembled from the parsed
 * stream of input tokens.
 * @param ch is the character to be appended.
 */
extern void assay_parser_section_next(int ch);

/**
 * Indicate the end of a section name in the parsed stream of input tokens.
 */
extern void assay_parser_section_end(void);

/*******************************************************************************
 * KEY
 ******************************************************************************/

/**
 * Indicate the beginning of a key in the parsed stream of input tokens.
 */
extern void assay_parser_key_begin(void);

/**
 * Append a character to the key being assembled from the parsed stream of input
 * tokens.
 * @param ch is the character to be appended.
 */
extern void assay_parser_key_next(int ch);

/**
 * Indicate the end of a key in the parsed stream of input tokens.
 */
extern void assay_parser_key_end(void);

/*******************************************************************************
 * VALUE
 ******************************************************************************/

/**
 * Indicate the beginning of a value in the parsed stream of input tokens.
 */
extern void assay_parser_value_begin(void);

/**
 * Append a character to the value being assembled from the parsed stream of
 * input tokens.
 * @param ch is the character to be appended.
 */
extern void assay_parser_value_next(int ch);

/**
 * Indicate the end of a value in the parsed stream of input tokens.
 */
extern void assay_parser_value_end(void);

/*******************************************************************************
 * PROPERTY
 ******************************************************************************/

/**
 * Assign a value to a property identified by key in a section using the
 * value, key, and section assembled from the parsed stream of input tokens.
 */
extern void assay_parser_property_assign(void);

#endif
