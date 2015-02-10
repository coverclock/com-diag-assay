%{

/**
 * @file
 * Assay Lexical Scanner
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include "assay_parser.h"
#include "com/diag/assay/assay_scanner_tools.h"

%}

%%

\\[0-3][0-7][0-7]                           { yylval = assay_scanner_text2value(yytext); return ESC; }
\\[0-7][0-7]                                { yylval = assay_scanner_text2value(yytext); return ESC; }
\\[0-7]                                     { yylval = assay_scanner_text2value(yytext); return ESC; }
\\x[0-9A-Fa-f][0-9A-Fa-f]                   { yylval = assay_scanner_text2value(yytext); return ESC; }
\\x[0-9A-Fa-f]                              { yylval = assay_scanner_text2value(yytext); return ESC; }
\\.                                         { yylval = assay_scanner_text2value(yytext); return ESC; }
[ \t]                                       { yylval = assay_scanner_text2value(yytext); return SP; }
"\n\r"                                      { yylval = assay_scanner_text2value(yytext); return EOL; }
"\n"                                        { yylval = assay_scanner_text2value(yytext); return EOL; }
"\r\n"                                      { yylval = assay_scanner_text2value(yytext); return EOL; }
"\r"                                        { yylval = assay_scanner_text2value(yytext); return EOL; }
"="                                         { yylval = assay_scanner_text2value(yytext); return EQ; }
":"                                         { yylval = assay_scanner_text2value(yytext); return EQ; }
";"                                         { yylval = assay_scanner_text2value(yytext); return SC; }
"["                                         { yylval = assay_scanner_text2value(yytext); return LB; }
"]"                                         { yylval = assay_scanner_text2value(yytext); return RB; }
.                                           { yylval = assay_scanner_text2value(yytext); return CH; }

%%

#if defined(COM_DIAG_ASSAY_SCANNER_TEST)

#include <ctype.h>

int main(int argc, int ** argv)
{
    int token;
    const char * name;

    do {
        token = yylex();
        name = assay_tools_token2name(token);
        if (isprint(yylval)) {
            fprintf(stderr, "\"%s\"[%zu] '%c' %d %s\n", yytext, strlen(yytext), yylval, token, name);
        } else if (isprint(*yytext)) {
            fprintf(stderr, "\"%s\"[%zu] \\x%2.2x %d %s\n", yytext, strlen(yytext), yylval, token, name);
        } else {
            fprintf(stderr, "\\x%2.2x[%zu] \\x%2.2x %d %s\n", *yytext, strlen(yytext), yylval, token, name);
        }
    } while (token != 0);
}

#endif
