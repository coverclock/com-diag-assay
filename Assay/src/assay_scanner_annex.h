/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_SCANNER_ANNEX_
#define _H_COM_DIAG_ASSAY_SCANNER_ANNEX_

/**
 * @file
 *
 * Copyright 2015-2020 Digital Aggregates Corporation, Colorado, USA
 * Licensed under the terms in LICENSE.txt.
 *
 * The Assay Scanner Annex feature is part of the private API.
 */

#include <stdio.h>

/*******************************************************************************
 * GENERATED
 ******************************************************************************/

extern int assay_scanner_yylex(/* FORWARD REFERENCE */);

/*******************************************************************************
 * TYPES
 ******************************************************************************/

/**
 * This is the opaque type returned, ultimately, by the lexical scanner
 * component generated by Flex.
 */
typedef void * assay_scanner_lexical_t;

/**
 * This is the type of the L-value (left-hand value, that is, the object that
 * represents the portion on the left-hand side of an expression to which a
 * value is assigned) returned by the lexical scanner component as a
 * value-result parameter.
 */
typedef int assay_scanner_lvalue_t;

/*******************************************************************************
 * SCANNER
 ******************************************************************************/

/**
 * Enable or disable debugging in the scanner and return the prior value.
 * @param enable is true to enable, false to disable.
 * @return the prior value.
 */
extern int assay_scanner_debug(int enable);

/**
 * Create a new lexical scanner and associate a configuration and an input
 * stream with it. Because this header file is included by the scanner generated
 * by Flex, the pointer to the configuration is opaque to avoid exposing the
 * Assay public interface to that component.
 * @param cfp points to the configuration.
 * @param stream is the input stream.
 * @return a pointer to the new lexical scanner.
 */
extern assay_scanner_lexical_t assay_scanner_create(void * cfp, FILE * stream);

/**
 * Destroy a lexical scanner. The configuration and stream associated with it
 * are unaffected and must be destroyed and closed by the application.
 * @param lxp points to the lexical scanner.
 */
extern void assay_scanner_destroy(assay_scanner_lexical_t lxp);

/**
 * Call the lexical scanner and return the next token it finds.
 * @param lvaluep points to the L-value variable.
 * @param lxp points to the lexical scanner.
 * @return the next token found.
 */
extern int assay_scanner_scan(assay_scanner_lvalue_t * lvaluep, assay_scanner_lexical_t lxp);

/**
 * Implement the yywrap() function. This is called by the lexical scanner as
 * it scans the input stream.
 * @param lxp points to the lexical scanner.
 * @return 0 to continue scanning or 1 for end of file.
 */
extern int assay_scanner_wrap(assay_scanner_lexical_t lxp);

/**
 * Increment the line number of the input file being processed. This is called
 * by the scanner as it scans the input stream.
 * @param lxp points to the lexical scanner.
 */
extern void assay_scanner_next(assay_scanner_lexical_t lxp);

/*******************************************************************************
 * HELPERS
 ******************************************************************************/

/**
 * Given a scanner token value, return a printable name for the token.
 * @param token is the scanner token value.
 * @return a printable name.
 */
extern const char * assay_scanner_token2name(int token);

/**
 * Given a text string, which may or may not be an escape sequence, assembled by
 * the scanner from its input stream, return a corresponding character.
 * @param text points to a text string assembled by the scanner.
 * @return a corresponding character value.
 */
extern int assay_scanner_text2value(const char * text);

#endif
