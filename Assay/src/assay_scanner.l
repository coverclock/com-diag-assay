%{

/**
 * @file
 * Assay Lexical Scanner
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * N.B. May not compile under Flex versions prior to 2.5.35!
 */

#include "assay_parser.h"
#include "assay_fixup.h"
#include "assay_scanner_annex.h"

%}

%option prefix="assay_scanner_yy"
%option reentrant
%option bison-bridge

%%

"\\\n\r"                                                                        { assay_scanner_next(yyscanner); }
"\\\n"                                                                          { assay_scanner_next(yyscanner); }
"\\\r\n"                                                                        { assay_scanner_next(yyscanner); }
"\\\r"                                                                          { assay_scanner_next(yyscanner); }
\\[0-3][0-7][0-7]                                                               { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\[0-7][0-7]                                                                    { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\[0-7]                                                                         { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\x[0-9A-Fa-f][0-9A-Fa-f]                                                       { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\x[0-9A-Fa-f]                                                                  { *yylval = assay_scanner_text2value(yytext); return ESC; }
\\.                                                                             { *yylval = assay_scanner_text2value(yytext); return ESC; }
[ \t]                                                                           { *yylval = assay_scanner_text2value(yytext); return SP;  }
"\n\r"                                                                          { *yylval = assay_scanner_text2value(yytext); return EOL; }
"\n"                                                                            { *yylval = assay_scanner_text2value(yytext); return EOL; }
"\r\n"                                                                          { *yylval = assay_scanner_text2value(yytext); return EOL; }
"\r"                                                                            { *yylval = assay_scanner_text2value(yytext); return EOL; }
"#"                                                                             { *yylval = assay_scanner_text2value(yytext); return OT;  }
"="                                                                             { *yylval = assay_scanner_text2value(yytext); return EQ;  }
":"                                                                             { *yylval = assay_scanner_text2value(yytext); return EQ;  }
";"                                                                             { *yylval = assay_scanner_text2value(yytext); return SC;  }
"["                                                                             { *yylval = assay_scanner_text2value(yytext); return LB;  }
"]"                                                                             { *yylval = assay_scanner_text2value(yytext); return RB;  }
"\x4"                                                                           { *yylval = assay_scanner_text2value(yytext); return END; }
.                                                                               { *yylval = assay_scanner_text2value(yytext); return CH;  }

%%

void assay_scanner_yy_set_interactive(yyscan_t yyscanner, int is_interactive)
{
    struct yyguts_t * yyg;
    yyg = (struct yyguts_t *)yyscanner;
    yy_set_interactive(is_interactive); /* This macro requires the yyg variable. */
}

int assay_scanner_yywrap(yyscan_t yyscanner)
{
    return assay_scanner_wrap(yyscanner);
}
