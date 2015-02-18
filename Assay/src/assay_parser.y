%{

/**
 * @file
 * Assay Parser Grammar and Actions
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include "com/diag/assay/assay_parser.h"

%}

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

operator_char:      CH                                                          { assay_parser_operator_next($1); }
                    | ESC                                                       { assay_parser_operator_next($1); }
                    ;

operator_tail:      operator_char
                    | operator_char operator_tail
                    ;

operator_init:      CH                                                          { assay_parser_operator_begin(); assay_parser_operator_next($1); }
                    | ESC                                                       { assay_parser_operator_begin(); assay_parser_operator_next($1); }
                    ;

operator_string:    operator_init
                    | operator_init operator_tail
                    ;

operator:           operator_string                                             { assay_parser_operator_end(); }
                    ;

argument_char:      CH                                                          { assay_parser_argument_next($1); }
                    | ESC                                                       { assay_parser_argument_next($1); }
                    ;

argument_tail:      argument_char
                    | argument_char argument_tail
                    ;

argument_init:      CH                                                          { assay_parser_argument_begin(); assay_parser_argument_next($1); }
                    | ESC                                                       { assay_parser_argument_begin(); assay_parser_argument_next($1); }
                    ;

argument_string:    argument_init
                    | argument_init argument_tail
                    ;

argument:           argument_string                                             { assay_parser_argument_end(); }
                    ;

operation:          OT operator whitespace argument                             { assay_parser_operation_execute(); }
                    | OT whitespace operator whitespace argument                { assay_parser_operation_execute(); }
                    ;

section_char:       CH                                                          { assay_parser_section_next($1); }
                    |  ESC                                                      { assay_parser_section_next($1); }
                    ;

section_tail:       section_char
                    | section_char section_tail
                    ;

section_init:       CH                                                          { assay_parser_section_begin(); assay_parser_section_next($1); }
                    | ESC                                                       { assay_parser_section_begin(); assay_parser_section_next($1); }
                    ;

section_string:     section_init
                    | section_init section_tail
                    ;

section:            LB section_string RB                                        { assay_parser_section_end(); }
                    ;

key_char:           CH                                                          { assay_parser_key_next($1); }
                    | ESC                                                       { assay_parser_key_next($1); }
                    ;

key_tail:           key_char
                    | key_char key_tail
                    ;

key_init:           CH                                                          { assay_parser_key_begin(); assay_parser_key_next($1); }
                    | ESC                                                       { assay_parser_key_begin(); assay_parser_key_next($1); }
                    ;

key_string:         key_init
                    | key_init key_tail
                    ;

key:                key_string                                                  { assay_parser_key_end(); }
                    ;

value_char:         CH                                                          { assay_parser_value_next($1); }
                    | ESC                                                       { assay_parser_value_next($1); }
                    | SP                                                        { assay_parser_value_next($1); }
                    ;

value_tail:         value_char
                    | value_char value_tail
                    ;

value_init:         CH                                                          { assay_parser_value_begin(); assay_parser_value_next($1); }
                    | ESC                                                       { assay_parser_value_begin(); assay_parser_value_next($1); }
                    ;

value_string:       value_init
                    | value_init value_tail
                    ;

value:              value_string                                                { assay_parser_value_end(); }
                    ;

assignment_tail:    EQ
                    | EQ value
                    | EQ whitespace value
                    ;

assignment:         key assignment_tail                                         { assay_parser_property_assign(); }
                    | key whitespace assignment_tail                            { assay_parser_property_assign(); }
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

statement:          EOL                                                         { assay_parser_next(); }
                    | comment EOL                                               { assay_parser_next(); }
                    | section EOL                                               { assay_parser_next(); }
                    | section comment EOL                                       { assay_parser_next(); }
                    | section whitespace EOL                                    { assay_parser_next(); }
                    | section whitespace comment EOL                            { assay_parser_next(); }
                    | section assignment EOL                                    { assay_parser_next(); }
                    | section assignment comment EOL                            { assay_parser_next(); }
                    | section whitespace assignment EOL                         { assay_parser_next(); }
                    | section whitespace assignment comment EOL                 { assay_parser_next(); }
                    | assignment EOL                                            { assay_parser_next(); }
                    | assignment comment EOL                                    { assay_parser_next(); }
                    | operation EOL                                             { assay_parser_next(); }
                    | operation comment EOL                                     { assay_parser_next(); }
                    | operation whitespace comment EOL                          { assay_parser_next(); }
                    | whitespace EOL                                            { assay_parser_next(); }
                    | whitespace comment EOL                                    { assay_parser_next(); }
                    | whitespace section EOL                                    { assay_parser_next(); }
                    | whitespace section comment EOL                            { assay_parser_next(); }
                    | whitespace section whitespace EOL                         { assay_parser_next(); }
                    | whitespace section whitespace comment EOL                 { assay_parser_next(); }
                    | whitespace section assignment EOL                         { assay_parser_next(); }
                    | whitespace section assignment comment EOL                 { assay_parser_next(); }
                    | whitespace section whitespace assignment EOL              { assay_parser_next(); }
                    | whitespace section whitespace assignment comment EOL      { assay_parser_next(); }
                    | whitespace assignment EOL                                 { assay_parser_next(); }
                    | whitespace assignment comment EOL                         { assay_parser_next(); }
                    | error EOL                                                 { assay_parser_next(); }
                    ;

statement_list:     statement
                    | statement_list statement
                    ;

file:               statement_list
                    ;

%%

int yyerror(char * msg)
{
    assay_parser_error(msg);
    return 0;
}
