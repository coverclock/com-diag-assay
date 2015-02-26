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
#include "assay_fixup.h"
#include "com/diag/assay/assay_scanner_annex.h"

int assay_scanner_yywrap(yyscan_t scanner)
{
    return assay_scanner_wrap(scanner);
}

%}

%option prefix="assay_scanner_yy"
%option reentrant
%option bison-bridge

%%

\\[0-3][0-7][0-7]                           { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\[0-7][0-7]                                { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\[0-7]                                     { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\x[0-9A-Fa-f][0-9A-Fa-f]                   { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\x[0-9A-Fa-f]                              { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\.                                         { *yylval = assay_scanner_text2value(yytext); return ESC; }
[ \t]                                       { *yylval = assay_scanner_text2value(yytext); return SP;  }
"\n\r"                                      { *yylval = assay_scanner_text2value(yytext); return EOL; }
"\n"                                        { *yylval = assay_scanner_text2value(yytext); return EOL; }
"\r\n"                                      { *yylval = assay_scanner_text2value(yytext); return EOL; }
"\r"                                        { *yylval = assay_scanner_text2value(yytext); return EOL; }
"#"                                         { *yylval = assay_scanner_text2value(yytext); return OT;  }
"="                                         { *yylval = assay_scanner_text2value(yytext); return EQ;  }
":"                                         { *yylval = assay_scanner_text2value(yytext); return EQ;  }
";"                                         { *yylval = assay_scanner_text2value(yytext); return SC;  }
"["                                         { *yylval = assay_scanner_text2value(yytext); return LB;  }
"]"                                         { *yylval = assay_scanner_text2value(yytext); return RB;  }
.                                           { *yylval = assay_scanner_text2value(yytext); return CH;  }

%%
