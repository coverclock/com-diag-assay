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

/******************************************************************************/

extern int assay_parser_debug(int enable);

/******************************************************************************/

extern void assay_parser_section_begin(void);

extern void assay_parser_section_next(int ch);

extern void assay_parser_section_end(void);

/******************************************************************************/

extern void assay_parser_key_begin(void);

extern void assay_parser_key_next(int ch);

extern void assay_parser_key_end(void);

/******************************************************************************/

extern void assay_parser_value_begin(void);

extern void assay_parser_value_next(int ch);

extern void assay_parser_value_end(void);

/******************************************************************************/

extern void assay_parser_property_commit(void);

/******************************************************************************/

#endif
