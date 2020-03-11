%{

/**
 * @file
 * Assay Parser Grammar and Actions
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 *
 * N.B. May not compile under Bison versions prior to 3.0.2!
 */

#include "assay_parser_annex.h"

%}

%define api.prefix {assay_parser_yy}
%define api.pure
%parse-param {void * scanner}
%lex-param {scanner}

%token END 0
%token CH
%token EOL
%token EQ
%token ESC
%token LB
%token OT
%token RB
%token SC
%token SP

%start file

%%

whitespace:         SP
                    | whitespace SP
                    ;

operator_char:      CH                                                          { assay_parser_operator_next(scanner, $1); }
                    | ESC                                                       { assay_parser_operator_next(scanner, $1); }
                    ;

operator_tail:      operator_char
                    | operator_char operator_tail
                    ;

operator_init:      CH                                                          { assay_parser_operator_begin(scanner); assay_parser_operator_next(scanner, $1); }
                    | ESC                                                       { assay_parser_operator_begin(scanner); assay_parser_operator_next(scanner, $1); }
                    ;

operator_string:    operator_init
                    | operator_init operator_tail
                    ;

operator:           operator_string                                             { assay_parser_operator_end(scanner); }
                    ;

argument_char:      CH                                                          { assay_parser_argument_next(scanner, $1); }
                    | ESC                                                       { assay_parser_argument_next(scanner, $1); }
                    | SP                                                        { assay_parser_argument_next(scanner, $1); }
                    ;

argument_tail:      argument_char
                    | argument_char argument_tail
                    ;

argument_init:      CH                                                          { assay_parser_argument_begin(scanner); assay_parser_argument_next(scanner, $1); }
                    | ESC                                                       { assay_parser_argument_begin(scanner); assay_parser_argument_next(scanner, $1); }
                    ;

argument_string:    argument_init
                    | argument_init argument_tail
                    ;

argument:           argument_string                                             { assay_parser_argument_end(scanner); }
                    ;

operation:          OT operator whitespace argument
                    | OT whitespace operator whitespace argument
                    ;

section_char:       CH                                                          { assay_parser_section_next(scanner, $1); }
                    |  ESC                                                      { assay_parser_section_next(scanner, $1); }
                    ;

section_tail:       section_char
                    | section_char section_tail
                    ;

section_init:       CH                                                          { assay_parser_section_begin(scanner); assay_parser_section_next(scanner, $1); }
                    | ESC                                                       { assay_parser_section_begin(scanner); assay_parser_section_next(scanner, $1); }
                    ;

section_string:     section_init
                    | section_init section_tail
                    ;

section:            LB section_string RB                                        { assay_parser_section_end(scanner); }
                    ;

key_char:           CH                                                          { assay_parser_key_next(scanner, $1); }
                    | ESC                                                       { assay_parser_key_next(scanner, $1); }
                    ;

key_tail:           key_char
                    | key_char key_tail
                    ;

key_init:           CH                                                          { assay_parser_key_begin(scanner); assay_parser_key_next(scanner, $1); }
                    | ESC                                                       { assay_parser_key_begin(scanner); assay_parser_key_next(scanner, $1); }
                    ;

key_string:         key_init
                    | key_init key_tail
                    ;

key:                key_string                                                  { assay_parser_key_end(scanner); }
                    ;

value_char:         CH                                                          { assay_parser_value_next(scanner, $1); }
                    | ESC                                                       { assay_parser_value_next(scanner, $1); }
                    | EQ                                                        { assay_parser_value_next(scanner, $1); }
                    | LB                                                        { assay_parser_value_next(scanner, $1); }
                    | OT                                                        { assay_parser_value_next(scanner, $1); }
                    | RB                                                        { assay_parser_value_next(scanner, $1); }
                    | SP                                                        { assay_parser_value_next(scanner, $1); }
                    ;

value_tail:         value_char
                    | value_char value_tail
                    ;

value_init:         CH                                                          { assay_parser_value_begin(scanner); assay_parser_value_next(scanner, $1); }
                    | ESC                                                       { assay_parser_value_begin(scanner); assay_parser_value_next(scanner, $1); }
                    | EQ                                                        { assay_parser_value_begin(scanner); assay_parser_value_next(scanner, $1); }
                    | LB                                                        { assay_parser_value_begin(scanner); assay_parser_value_next(scanner, $1); }
                    | OT                                                        { assay_parser_value_begin(scanner); assay_parser_value_next(scanner, $1); }
                    | RB                                                        { assay_parser_value_begin(scanner); assay_parser_value_next(scanner, $1); }
                    ;

value_string:       value_init
                    | value_init value_tail
                    ;

value:              value_string                                                { assay_parser_value_end(scanner); }
                    ;

assignment_tail:    EQ
                    | EQ value
                    | EQ whitespace value
                    ;

assignment:         key assignment_tail
                    | key whitespace assignment_tail
                    ;

comment_char:       CH
                    | ESC
                    | EQ
                    | LB
                    | OT
                    | RB
                    | SC
                    | SP
                    ;

comment_string:     comment_char
                    | comment_string comment_char
                    ;

comment:            SC
                    | SC comment_string
                    ;

statement:          EOL                                                         { assay_parser_next(scanner); }
                    | comment EOL                                               { assay_parser_next(scanner); }
                    | section EOL                                               { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | section comment EOL                                       { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | section whitespace EOL                                    { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | section whitespace comment EOL                            { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | section assignment EOL                                    { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | section assignment comment EOL                            { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | section whitespace assignment EOL                         { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | section whitespace assignment comment EOL                 { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | assignment EOL                                            { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | assignment comment EOL                                    { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | whitespace EOL                                            { assay_parser_next(scanner); }
                    | whitespace comment EOL                                    { assay_parser_next(scanner); }
                    | whitespace section EOL                                    { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | whitespace section comment EOL                            { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | whitespace section whitespace EOL                         { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | whitespace section whitespace comment EOL                 { assay_parser_next(scanner); /* Section only created if property is assigned within it. */ }
                    | whitespace section assignment EOL                         { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | whitespace section assignment comment EOL                 { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | whitespace section whitespace assignment EOL              { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | whitespace section whitespace assignment comment EOL      { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | whitespace assignment EOL                                 { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | whitespace assignment comment EOL                         { assay_parser_next(scanner); assay_parser_property_assign(scanner); }
                    | operation EOL                                             { assay_parser_next(scanner); assay_parser_operation_execute(scanner); }
                    | operation comment EOL                                     { assay_parser_next(scanner); assay_parser_operation_execute(scanner); }
                    | error EOL                                                 { assay_parser_next(scanner); }
                    ;

statement_list:     statement
                    | statement_list statement
                    ;

file:               statement_list
                    ;

%%

int assay_parser_yylex(YYSTYPE * lval, void * scanner)
{
    return assay_scanner_yylex(lval, scanner);
}

int assay_parser_yyerror(void * lxp, char * msg)
{
    assay_parser_error(lxp, msg);
    return 0;
}
