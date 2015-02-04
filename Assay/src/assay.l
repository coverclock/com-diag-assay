%{

enum yytokentype {
    ZERO        = '0',
    EQUAL       = '=',
    COLON       = ':',
    SEMICOLON   = ';',
    OCTOTHORPE  = '#',
    ESCAPE      = '\\',
    LEFT        = '[',
    RIGHT       = ']',
    DOUBLE      = '"',
    SINGLE      = '\'',
    WHITESPACE  = ' ',
    NL          = '\n',
};

%}

%%

"0"     { return ZERO; }
"="     { return EQUAL; }
":"     { return COLON; }
";"     { return SEMICOLON; }
"#"     { return OCTOTHORPE; }
"\\"    { return ESCAPE; }
"["     { return LEFT; }
"]"     { return RIGHT; }
"\""    { return DOUBLE; }
"'"     { return SINGLE; }
[ \t]   { return WHITESPACE; }
"\n"    { return NL; }
.       { return *yytext; }

%%

main(int argc, int ** argv)
{
    int tok;

    while ((tok = yylex()) != '\0') {
        printf("%d\n", tok);
    }
}
