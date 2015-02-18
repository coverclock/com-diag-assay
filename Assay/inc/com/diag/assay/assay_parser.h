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

/**
 * Record the name of the input file. This is used for subsequent error
 * messages.
 * @param path is the path to the input file.
 * @return the path to the prior input file.
 */
extern const char * assay_parser_file(const char * path);

/**
 * Record the beginning line number of the input file. This is used for
 * subsequent error messages.
 * @param origin is the beginning line number of the input file (typically zero).
 * @return the prior line number.
 */
extern int assay_parser_line(int origin);

/**
 * Increment the line number of the input file being processed. This is called
 * by the parser as it parses the input file.
 */
extern void assay_parser_next(void);

/**
 * Indicate that a parse error has occurred. This is called by the parser
 * as it parses the input file.
 * @param msg is a message passed from the parser.
 */
extern void assay_parser_error(const char * msg);

/*******************************************************************************
 * OPERATOR
 ******************************************************************************/

/**
 * Indicate the beginning of an operator in the parsed stream of input
 * tokens.
 */
extern void assay_parser_operator_begin(void);

/**
 * Append a character to the operator being assembled from the parsed
 * stream of input tokens.
 * @param ch is the character to be appended.
 */
extern void assay_parser_operator_next(int ch);

/**
 * Indicate the end of a operator in the parsed stream of input tokens.
 */
extern void assay_parser_operator_end(void);

/*******************************************************************************
 * ARGUMENT
 ******************************************************************************/

/**
 * Indicate the beginning of an operator argument in the parsed stream of input
 * tokens.
 */
extern void assay_parser_argument_begin(void);

/**
 * Append a character to the operator argument being assembled from the parsed
 * stream of input tokens.
 * @param ch is the character to be appended.
 */
extern void assay_parser_argument_next(int ch);

/**
 * Indicate the end of a operator argument in the parsed stream of input
 * tokens.
 */
extern void assay_parser_argument_end(void);

/*******************************************************************************
 * OPERATION
 ******************************************************************************/

/**
 * Execute an operation using the operator and argument assembled from the
 * parsed stream of input tokens.
 */
extern void assay_parser_operation_execute(void);

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
