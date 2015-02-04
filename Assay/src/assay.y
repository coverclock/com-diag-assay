/**
 * @file
 * Assay LALR(1) Parser
 * Copyright 2015 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Assay.html<BR>
 */



%token UNK
%token CO
%token DEC
%token DQ
%token EQ
%token ESC
%token ESC_LC
%token HEX_LC
%token HEX_UC
%token LB
%token LC
%token NL
%token OCT
%token OCT_INI
%token OT
%token PUNCT
%token RB
%token SC
%token SP
%token SQ
%token UC
%token X_LC
%token X_UC



%%



lowercase:          LC |
                    HEX_LC |
                    X_LC;

uppercase:          UC |
                    HEX_UC |
                    X_UC;

digit:              OCT_INI |
                    OCT |
                    DEC

special:            PUNCT |
                    DQ |
                    OT |
                    SQ |
                    ESC |
                    CO |
                    SC |
                EQ |
                    LB |
                    RB;

any:                lowercase |
                    uppercase |
                    digit |
                    special |
                    SP;



oct_dig:            OCT_INI |
                    OCT;

oct_tail:           oct_dig |
                    oct_dig oct_dig;

oct_num:            OCT_INI oct_tail;



hex_dig:            oct_dig |
                    DEC |
                    HEX_LC |
                    HEX_UC;

hex_pfx:            X_LC |
                    X_UC;

hex_num:            hex_pfx hex_dig hex_dig;



esc_arg:            ESC |
                    ESC_LC |
                    SC |
                    oct_num |
                    hex_num;

esc_seq:            ESC esc_arg;



whitespace:         whitespace SP |
                    SP;



name_char:          lowercase |
                    uppercase |
                    digit |
                    PUNCT;

name_chars:         name_char name_chars |
                    name_char;

name:               name_chars;

section_tail:       whitespace RB |
                    RB;

section_middle:     whitespace name section_tail |
                    name section_tail;

section:            LB section_middle;



equals:             EQ | 
                    CO;

keyword_tail:       whitespace equals |
                    equals;

keyword:            name keyword_tail;

value_tail:         lowercase |
                    lowercase value_tail |
                    uppercase |
                    uppercase value_tail |
                    digit |
                    digit value_tail |
                    special |
                    special value_tail |
                    SP |
                    SP value_tail;

value_middle:       lowercase |
                    lowercase value_tail |
                    uppercase |
                    uppercase value_tail |
                    digit |
                    digit value_tail |
                    special |
                    special value_tail;

value:              value_middle;

assignment_tail:    value |
                    whitespace value;

assignment:         keyword assignment_tail;



comment_tail:       any |
                    any comment_tail;

comment:            SC |
                    SC comment_tail;



statement:          comment |
                    section | 
                    assignment;



line_tail:          NL |
                    comment NL;

line_middle:        line_tail |
                    whitespace line_tail;

line:               line_middle |
                    statement line_middle |
                    whitespace statement line_middle;



%%
