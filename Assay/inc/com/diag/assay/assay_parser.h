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
 * Increment the line number of the input file being processed. This is called
 * by the parser as it parses the input file.
 * @param cfp points to the configuration.
 */
extern void assay_parser_next(void * lxp);

/**
 * Indicate that a parse error has occurred. This is called by the parser
 * as it parses the input file.
 * @param lxp points to the lexical scanner.
 * @param msg is a message passed from the parser.
 */
extern void assay_parser_error(void * lxp, const char * msg);

/*******************************************************************************
 * OPERATOR
 ******************************************************************************/

/**
 * Indicate the beginning of an operator in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_operator_begin(void * lxp);

/**
 * Append a character to the operator being assembled from the parsed
 * stream of input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_operator_next(void * lxp, int ch);

/**
 * Indicate the end of a operator in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_operator_end(void * lxp);

/*******************************************************************************
 * ARGUMENT
 ******************************************************************************/

/**
 * Indicate the beginning of an operator argument in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_argument_begin(void * lxp);

/**
 * Append a character to the operator argument being assembled from the parsed
 * stream of input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_argument_next(void * lxp, int ch);

/**
 * Indicate the end of a operator argument in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_argument_end(void * lxp);

/*******************************************************************************
 * OPERATION
 ******************************************************************************/

/**
 * Execute an operation using the operator and argument assembled from the
 * parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_operation_execute(void * lxp);

/*******************************************************************************
 * SECTION
 ******************************************************************************/

/**
 * Indicate the beginning of a section name in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_section_begin(void * lxp);

/**
 * Append a character to the section name being assembled from the parsed
 * stream of input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_section_next(void * lxp, int ch);

/**
 * Indicate the end of a section name in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_section_end(void * lxp);

/*******************************************************************************
 * KEY
 ******************************************************************************/

/**
 * Indicate the beginning of a key in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_key_begin(void * lxp);

/**
 * Append a character to the key being assembled from the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_key_next(void * lxp, int ch);

/**
 * Indicate the end of a key in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_key_end(void * lxp);

/*******************************************************************************
 * VALUE
 ******************************************************************************/

/**
 * Indicate the beginning of a value in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_value_begin(void * lxp);

/**
 * Append a character to the value being assembled from the parsed stream of
 * input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_value_next(void * lxp, int ch);

/**
 * Indicate the end of a value in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_value_end(void * lxp);

/*******************************************************************************
 * PROPERTY
 ******************************************************************************/

/**
 * Assign a value to a property identified by key in a section using the
 * value, key, and section assembled from the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_property_assign(void * lxp);

#endif
