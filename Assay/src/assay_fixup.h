/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.
 *
 * Bison and Flex disagree on what this preprocessor symbol should be called
 * when both components have a special API prefix specified.
 */

#if !defined(YYSTYPE)
#   define YYSTYPE ASSAY_PARSER_YYSTYPE
#endif
