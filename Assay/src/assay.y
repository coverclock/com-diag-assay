lowercase:      LC |
                HEX_LC |
                X_LC;

uppercase:      UC |
                HEX_UC |
                X_UC;

digit:          OCT_INI |
                OCT |
                DEC

special:        PUNCT |
                DQ |
                OT |
                SQ |
                ESC |
                CO |
                SC |
                EQ |
                LB |
                RB;

any:            lowercase |
                uppercase |
                digit |
                special |
                SP;



oct_dig:        OCT_INI |
                OCT;

oct_tail:       oct_dig |
                oct_dig oct_dig;

oct_num:        OCT_INI oct_tail;



hex_dig:        oct_digit |
                DEC |
                HEX_LC |
                HEX_UC;

hex_pfx:        X_LC |
                X_UC;

hex_num:        hex_pfx hex_dig hex_dig;



esc_arg:        ESC |
                ESC_LC |
                SC |
                oct_num |
                hex_num;

esc_seq:        ESC esc_arg;



whitespace:     whitespace SP |
                SP;



name_char:      lowercase |
                uppercase |
                digit |
                PUNCT;

name_chars:     name_char name_chars |
                name_char;

name:           name_chars;



DOUBLE_char: lowercase | uppercase | dec_dig | esc_seq | WHITESPACE | '!' | OCTOTHORPE | '$' | '%' | '&' | SINGLE | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | SEMICOLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

DOUBLE_quoted: DOUBLE_char DOUBLE_quoted | nil;

SINGLE_char: lowercase | uppercase | dec_dig | esc_seq | WHITESPACE | '!' | DOUBLE | OCTOTHORPE | '$' | '%' | '&' | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | SEMICOLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

SINGLE_quoted: SINGLE_char SINGLE_quoted | nil;

unquoted_char_init: lowercase | uppercase | dec_dig | esc_seq | '!' | '$' | '%' | '&' | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

unquoted_char: lowercase | uppercase | dec_dig | esc_seq | WHITESPACE | '!' | DOUBLE | '$' | '%' | '&' | SINGLE | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

unquoted_chars: unquoted_char unquoted_chars | nil;

unquoted: unquoted_char_init unquoted_chars;

value: DOUBLE DOUBLE_quoted DOUBLE | SINGLE SINGLE_quoted SINGLE | unquoted;


section_tail:   whitespace RB |
                RB;

section_middle: whitespace name section_tail |
                name section_tail;

section:        LB section_middle;



assign:         EQ | 
                CO;

keyword_tail:   whitespace assign |
                assign;

keyword:        name keyword_tail;

value_tail:     lowercase |
                lowercase value_tail |
                uppercase |
                uppercase value_tail |
                digit |
                digit value_tail |
                special |
                special value_tail |
                SP |
                SP value_tail;

value_middle:   lowercase |
                lowercase value_tail |
                uppercase |
                uppercase value_tail |
                digit |
                digit value_tail |
                special |
                special value_tail;

value:          value_middle;

assignment:     keyword whitespaces value;



comment: SC skip;

commented: comment | nil;

statement: comment | section | assignment | nil;

line:       statement NL |
            statement whitespace NL |
            whitespace statement NL |
            ;
