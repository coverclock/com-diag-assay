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

/**
 * Enable or disable debugging in the scanner and return the prior value.
 * @param enable is true to enable, false to disable.
 * @return the prior value.
 */
extern int assay_scanner_debug(int enable);

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

/**
 * Implement the yywrap() function called by the scanner.
 * @param lxp points to the lexical scanner.
 * @return 0 to continue scanning or 1 for end of file.
 */
extern int assay_scanner_wrap(void * lxp);

#endif
