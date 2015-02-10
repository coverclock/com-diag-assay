%{

/**
 * @file
 * Assay Parser Grammar and Actions
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */

#include <stdio.h>
#include "com/diag/assay/assay_scanner.h"

%}

%token CH
%token EOL
%token EQ
%token ESC
%token LB
%token RB
%token SC
%token SP

%start file

%%

whitespace:         SP
                    | whitespace SP
                    ;

section_char:       CH { printf("section_char '%c'\n", $1); }
                    | ESC { printf("section_char '%c'\n", $1); }
                    ;

section_tail:       section_char
                    | section_char section_tail
                    ;

section_init:       CH { printf("section_init '%c'\n", $1); }
                    | ESC { printf("section_init '%c'\n", $1); }
                    ;

section_string:     section_init
                    | section_init section_tail
                    ;

section:            LB section_string RB
                    ;

keyword_char:       CH { printf("keyword_char '%c'\n", $1); }
                    | ESC { printf("keyword_char '%c'\n", $1); }
                    ;

keyword_tail:       keyword_char
                    | keyword_char keyword_tail
                    ;

keyword_init:       CH { printf("keyword_init '%c'\n", $1); }
                    | ESC { printf("keyword_init '%c'\n", $1); }
                    ;

keyword:            keyword_init
                    | keyword_init keyword_tail
                    ;

value_char:         CH { printf("value_char '%c'\n", $1); }
                    | ESC { printf("value_char '%c'\n", $1); }
                    | SP { printf("value_char '%c'\n", $1); }
                    ;

value_tail:         value_char
                    | value_char value_tail
                    ;

value_init:         CH { printf("value_init '%c'\n", $1); }
                    | ESC { printf("value_init '%c'\n", $1); }
                    ;

value:              value_init
                    | value_init value_tail
                    ;

assignment_tail:    EQ
                    | EQ value
                    ;

assignment:         keyword assignment_tail
                    ;

comment_char:       CH
                    | ESC
                    | EQ
                    | LB
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

statement:          EOL
                    | comment EOL
                    | section EOL
                    | section comment EOL
                    | section whitespace EOL
                    | section whitespace comment EOL
                    | section assignment EOL
                    | section assignment comment EOL
                    | section whitespace assignment EOL
                    | section whitespace assignment comment EOL
                    | assignment EOL
                    | assignment comment EOL
                    | whitespace EOL
                    | whitespace comment EOL
                    | whitespace section EOL
                    | whitespace section comment EOL
                    | whitespace section whitespace EOL
                    | whitespace section whitespace comment EOL
                    | whitespace section assignment EOL
                    | whitespace section assignment comment EOL
                    | whitespace section whitespace assignment EOL
                    | whitespace section whitespace assignment comment EOL
                    | whitespace assignment EOL
                    | whitespace assignment comment EOL
                   ;

statement_list:     statement
                    | statement_list statement
                    ;

file:               statement_list
                    ;

%%
