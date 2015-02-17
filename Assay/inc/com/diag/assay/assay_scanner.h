/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_SCANNER_ANNEX_
#define _H_COM_DIAG_ASSAY_SCANNER_ANNEX_

/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include <stdio.h>

/**
 * Enable or disable debugging in the scanner and return the prior value.
 * @param enable is true to enable, false to disable.
 * @return the prior value.
 */
extern int assay_scanner_debug(int enable);

/**
 * Set the input stream for the scanner and return the prior value.
 * @param fp points to the new input scanner stream.
 * @return the pointer to the prior input scanner stream.
 */
extern FILE * assay_scanner_input(FILE * fp);

/**
 * Do whatever is necessary to place the scanner in a known state following
 * presumably an error in the parse.
 */
extern void assay_scanner_error();

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
