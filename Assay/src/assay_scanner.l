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
#include "com/diag/assay/assay_scanner.h"

extern int assay_yylex (int yylval);
#define YY_DECL int assay_yylex (int yylval)

int assay_yywrap() {
    return assay_scanner_wrap();
}

%}

%option prefix="assay_yy"

%%

\\[0-3][0-7][0-7]                           { yylval = assay_scanner_text2value(assay_yytext); return ESC; }
\\[0-7][0-7]                                { yylval = assay_scanner_text2value(assay_yytext); return ESC; }
\\[0-7]                                     { yylval = assay_scanner_text2value(assay_yytext); return ESC; }
\\x[0-9A-Fa-f][0-9A-Fa-f]                   { yylval = assay_scanner_text2value(assay_yytext); return ESC; }
\\x[0-9A-Fa-f]                              { yylval = assay_scanner_text2value(assay_yytext); return ESC; }
\\.                                         { yylval = assay_scanner_text2value(assay_yytext); return ESC; }
[ \t]                                       { yylval = assay_scanner_text2value(assay_yytext); return SP; }
"\n\r"                                      { yylval = assay_scanner_text2value(assay_yytext); return EOL; }
"\n"                                        { yylval = assay_scanner_text2value(assay_yytext); return EOL; }
"\r\n"                                      { yylval = assay_scanner_text2value(assay_yytext); return EOL; }
"\r"                                        { yylval = assay_scanner_text2value(assay_yytext); return EOL; }
"#"                                         { yylval = assay_scanner_text2value(assay_yytext); return OT; }
"="                                         { yylval = assay_scanner_text2value(assay_yytext); return EQ; }
":"                                         { yylval = assay_scanner_text2value(assay_yytext); return EQ; }
";"                                         { yylval = assay_scanner_text2value(assay_yytext); return SC; }
"["                                         { yylval = assay_scanner_text2value(assay_yytext); return LB; }
"]"                                         { yylval = assay_scanner_text2value(assay_yytext); return RB; }
.                                           { yylval = assay_scanner_text2value(assay_yytext); return CH; }

%%
