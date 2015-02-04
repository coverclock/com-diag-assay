whitespaces: WHITESPACE whitespaces | nil;

/********************/;

oct_dig_first: ZERO | '1' | '2' | '3';

oct_dig: oct_dig_first | '4' | '5' | '6' | '7';

dec_dig: octal_dig | '8' | '9';

a_or_b_lc: 'a' | 'b';

hex_dig_lc: a_or_b_lc | 'c' | 'd' | 'e' | 'f';

hex_dig_uc: 'A' | 'B' | 'C' | 'D' | 'E' | 'F';

hex_dig: dec_dig | hex_dig_lc | hex_dig_uc;

oct_num: ZERO oct_dig_first oct_dig oct_dig;

hex_pfx_lc: 'x';

hex_pfx_uc: 'X';

hex_pfx: hex_pfx_lc | hex_pfx_uc;

hex_num: hex_pfx hex_dig hex_dig hex_dig hex_dig;

esc_arg: ESCAPE | ZERO | a_or_b_lc | 't' | 'r' | 'n' | SEMICOLON | OCTOTHORPE | EQUAL | COLON | LEFT | RIGHT | DOUBLE | SINGLE | oct_num | hex_num;

esc_seq: ESCAPE esc_arg;

/********************/;

lc: hex_digit_lc | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | hex_pfx_lc | 'y' | 'z';

uc: hex_digit_uc | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | hex_pfx_uc | 'Y' | 'Z';

special: '!' | DOUBLE | OCTOTHORPE | '$' | '%' | '&' | SINGLE | '(' | ')' | '*' | '+' | '`' | '-' | '.' | ESCAPE | COLON | SEMICOLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | ESCAPE | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

any: lc | uc | dec_dig | special | WHITESPACE;

skip: any skip | nil;

/********************/;

name_char: esc_seq | lc | uc | dec_dig | '!' | '$' | '%' | '&' | '(' | ')' | '*' | '+' | '`' | '-' | '.' | '/' | '<' | '>' | '?' | '@' | '^' | '_' | '{' | '|' | '{' | '~';

name_chars: name_char name_chars | nil;

name: name_char name_chars;

/********************/;

DOUBLE_char: lc | uc | dec_dig | esc_seq | WHITESPACE | '!' | OCTOTHORPE | '$' | '%' | '&' | SINGLE | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | SEMICOLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

DOUBLE_quoted: DOUBLE_char DOUBLE_quoted | nil;

SINGLE_char: lc | uc | dec_dig | esc_seq | WHITESPACE | '!' | DOUBLE | OCTOTHORPE | '$' | '%' | '&' | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | SEMICOLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

SINGLE_quoted: SINGLE_char SINGLE_quoted | nil;

unquoted_char_init: lc | uc | dec_dig | esc_seq | '!' | '$' | '%' | '&' | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

unquoted_char: lc | uc | dec_dig | esc_seq | WHITESPACE | '!' | DOUBLE | '$' | '%' | '&' | SINGLE | '(' | ')' | '*' | '+' | '`' | '-' | '.' | COLON | '<' | EQUAL | '>' | '?' | '@' | LEFT | RIGHT | '^' | '_' | '{' | '|' | '{' | '~';

unquoted_chars: unquoted_char unquoted_chars | nil;

unquoted: unquoted_char_init unquoted_chars;

value: DOUBLE DOUBLE_quoted DOUBLE | SINGLE SINGLE_quoted SINGLE | unquoted;

/********************/;

section: LEFT whitespaces name whitespaces RIGHT;

assign: EQUAL | COLON;

keyword: name whitespaces assign;

assignment: keyword whitespaces value;

comment: SEMICOLON skip | OCTOTHORPE skip;

commented: comment | nil;

statement: comment | section | assignment | nil;

line: whitespaces statement commented;
