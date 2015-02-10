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

static const char ASSAY_PARSER_SECTION_DEFAULT[] = "general";

/******************************************************************************/

extern void assay_parser_section_begin(int ch);

extern void assay_parser_section_next(int ch);

extern void assay_parser_section_end(void);

/******************************************************************************/

extern void assay_parser_keyword_begin(int ch);

extern void assay_parser_keyword_next(int ch);

extern void assay_parser_keyword_end(void);

/******************************************************************************/

extern void assay_parser_value_begin(int ch);

extern void assay_parser_value_next(int ch);

extern void assay_parser_value_end(void);

/******************************************************************************/

#endif
