%{

/**
 * @file
 * Assay Lexical Scanner
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

enum yytokentype {
    UNK         = 258,
    CO,
    DEC,
    DQ,
    EQ,
    ESC,
    ESC_LC,
    HEX_LC,
    HEX_UC,
    LB,
    LC,
    NL,
    OCT,
    OCT_INI,
    OT,
    PUNCT,
    RB,
    SC,
    SP,
    SQ,
    UC,
    X_LC,
    X_UC,
};

#if defined(COM_DIAG_ASSAY_LEX_TEST) && COM_DIAG_ASSAY_LEX_TEST

static const char * yytokenname[] = {
    "END",
    "UNK",
    "CO",
    "DEC",
    "DQ",
    "EQ",
    "ESC",
    "ESC_LC",
    "HEX_LC",
    "HEX_UC",
    "LB",
    "LC",
    "NL",
    "OCT",
    "OCT_INI",
    "OT",
    "PUNCT",
    "RB",
    "SC",
    "SP",
    "SQ",
    "UC",
    "X_LC",
    "X_UC",
};

static char yytokennext = 0;

#define YY_INPUT(_BUFFER_, _RESULT_, _SIZE_) do { *_BUFFER_ = ++yytokennext; _RESULT_ = (yytokennext != '\0') ? 1 : 0; } while (0)

#endif

%}

%%

[ \t\f\v]                               { return SP; }
"\n"                                    { return NL; }
"\r"                                    { return NL; }
[0-3]                                   { return OCT_INI; }
[4-7]                                   { return OCT; }
[8-9]                                   { return DEC; }
[A-F]                                   { return HEX_UC; }
[G-W]                                   { return UC; }
"X"                                     { return X_UC; }
[Y-Z]                                   { return UC; }
[a-b]                                   { return ESC_LC; }
[c-f]                                   { return HEX_LC; }
[g-m]                                   { return LC; }
"n"                                     { return ESC_LC; }
[o-q]                                   { return LC; }
"r"                                     { return ESC_LC; }
"s"                                     { return LC; }
"t"                                     { return ESC_LC; }
[u-w]                                   { return LC; }
"x"                                     { return X_LC; }
[y-z]                                   { return LC; }
[!\$%&\(\)\*\+,`\-\.\/<>\?@\^_\{\|\}~]  { return PUNCT; }
"="                                     { return EQ; }
":"                                     { return CO; }
";"                                     { return SC; }
"#"                                     { return OT; }
"\\"                                    { return ESC; }
"["                                     { return LB; }
"]"                                     { return RB; }
"\""                                    { return DQ; }
"'"                                     { return SQ; }
.                                       { return UNK; }

%%

#if defined(COM_DIAG_ASSAY_LEX_TEST) && COM_DIAG_ASSAY_LEX_TEST

#include <ctype.h>

main(int argc, int ** argv)
{
    int tok;
    unsigned char ch;

    do {
        tok = yylex();
        ch = *yytext;
        switch (ch) {
        case '\0':
            fprintf(stderr, "'\\%c' %d %s\n", '0', tok, yytokenname[tok]);
            break;
        case '\a':
            fprintf(stderr, "'\\%c' %d %s\n", 'a', tok, yytokenname[tok]);
            break;
        case '\b':
            fprintf(stderr, "'\\%c' %d %s\n", 'b', tok, yytokenname[tok]);
            break;
        case '\t':
            fprintf(stderr, "'\\%c' %d %s\n", 't', tok, yytokenname[tok]);
            break;
        case '\n':
            fprintf(stderr, "'\\%c' %d %s\n", 'n', tok, yytokenname[tok]);
            break;
        case '\v':
            fprintf(stderr, "'\\%c' %d %s\n", 'v', tok, yytokenname[tok]);
            break;
        case '\f':
            fprintf(stderr, "'\\%c' %d %s\n", 'f', tok, yytokenname[tok]);
            break;
        case '\r':
            fprintf(stderr, "'\\%c' %d %s\n", 'r', tok, yytokenname[tok]);
            break;
        case '\'':
            fprintf(stderr, "'\\%c' %d %s\n", '\'', tok, yytokenname[tok]);
            break;
        case '\\':
            fprintf(stderr, "'\\%c' %d %s\n", '\\', tok, yytokenname[tok]);
            break;
        default:
            if (isprint(ch)) {
                fprintf(stderr, "'%c' %d %s\n", ch, tok, yytokenname[tok]);
            } else {
                fprintf(stderr, "\\x%2.2x %d %s\n", ch, tok, yytokenname[tok]);
            }
            break;
        }
    } while (tok != 0);
}

#endif
