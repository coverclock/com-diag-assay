/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_PARSER_ANNEX_
#define _H_COM_DIAG_ASSAY_PARSER_ANNEX_

/**
 * @file
 *
 * Copyright 2015-2020 Digital Aggregates Corporation, Colorado, USA
 * Licensed under the terms in README.h
 *
 * The Assay Parser Annex feature is part of the private API.
 */

#include "assay_scanner_annex.h"

/*******************************************************************************
 * GENERATED
 ******************************************************************************/

extern int assay_parser_yylex(/* FORWARD REFERENCE */);

extern int assay_parser_yyerror(/* FORWARD REFERENCE */);

/*******************************************************************************
 * PARSER
 ******************************************************************************/

/**
 * Enable or disable debugging in the parser and return the prior value.
 * @param enable is true to enable, false to disable.
 * @return the prior value.
 */
extern int assay_parser_debug(int enable);

/**
 * Call the shift-reduce parser to parse the input stream associated with the
 * specified lexical scanner.
 * @return 0 for accept, 1 for reject, 2 for out of memory.
 */
extern int assay_parser_parse(assay_scanner_lexical_t lxp);

/**
 * Increment the line number of the input file being processed. This is called
 * by the parser as it parses the input file.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_next(assay_scanner_lexical_t lxp);

/**
 * Indicate that a parse error has occurred. This is called by the parser
 * as it parses the input file.
 * @param lxp points to the lexical scanner.
 * @param msg is a message passed from the parser.
 */
extern void assay_parser_error(assay_scanner_lexical_t lxp, const char * msg);

/**
 * Free memory allocated temporarily for buffers used while parsing. This is
 * called by the public API after importing.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_fini(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * OPERATOR
 ******************************************************************************/

/**
 * Indicate the beginning of an operator in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_operator_begin(assay_scanner_lexical_t lxp);

/**
 * Append a character to the operator being assembled from the parsed
 * stream of input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_operator_next(assay_scanner_lexical_t lxp, int ch);

/**
 * Indicate the end of a operator in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_operator_end(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * ARGUMENT
 ******************************************************************************/

/**
 * Indicate the beginning of an operator argument in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_argument_begin(assay_scanner_lexical_t lxp);

/**
 * Append a character to the operator argument being assembled from the parsed
 * stream of input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_argument_next(assay_scanner_lexical_t lxp, int ch);

/**
 * Indicate the end of a operator argument in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_argument_end(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * OPERATION
 ******************************************************************************/

/**
 * Execute an operation using the operator and argument assembled from the
 * parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_operation_execute(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * SECTION
 ******************************************************************************/

/**
 * Indicate the beginning of a section name in the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_section_begin(assay_scanner_lexical_t lxp);

/**
 * Append a character to the section name being assembled from the parsed
 * stream of input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_section_next(assay_scanner_lexical_t lxp, int ch);

/**
 * Indicate the end of a section name in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_section_end(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * KEY
 ******************************************************************************/

/**
 * Indicate the beginning of a key in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_key_begin(assay_scanner_lexical_t lxp);

/**
 * Append a character to the key being assembled from the parsed stream of input
 * tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_key_next(assay_scanner_lexical_t lxp, int ch);

/**
 * Indicate the end of a key in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_key_end(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * VALUE
 ******************************************************************************/

/**
 * Indicate the beginning of a value in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_value_begin(assay_scanner_lexical_t lxp);

/**
 * Append a character to the value being assembled from the parsed stream of
 * input tokens.
 * @param lxp points to the lexical scanner.
 * @param ch is the character to be appended.
 */
extern void assay_parser_value_next(assay_scanner_lexical_t lxp, int ch);

/**
 * Indicate the end of a value in the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_value_end(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * PROPERTY
 ******************************************************************************/

/**
 * Assign a value to a property identified by key in a section using the
 * value, key, and section assembled from the parsed stream of input tokens.
 * @param lxp points to the lexical scanner.
 */
extern void assay_parser_property_assign(assay_scanner_lexical_t lxp);

#endif
