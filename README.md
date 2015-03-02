com-diag-assay
==============

Copyright 2015 by the Digital Aggregates Corporation, Colorado, USA.

LICENSE

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

As a special exception, if other files instantiate templates or
use macros or inline functions from this file, or you compile
this file and link it with other works to produce a work based on
this file, this file does not by itself cause the resulting work
to be covered by the GNU Lesser General Public License. However
the source code for this file must still be made available in
accordance with the GNU Lesser General Public License.

This exception does not invalidate any other reasons why a work
based on this file might be covered by the GNU Lesser General
Public License.

Alternative commercial licensing terms are available from the copyright
holder. Contact Digital Aggregates Corporation for more information.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, contact

    Free Software Foundation, Inc.
    59 Temple Place, Suite 330
    Boston MA 02111-1307 USA
    http://www.gnu.org/copyleft/lesser.txt

ABSTRACT

This file is part of the Digital Aggregates Corporation Assay package.
Assay is a C library that provides functions to parse a configuration file
in yet one more variation on the widely used and under specified INI
format. The syntax of this particular format is specified by an LALR(1)
grammar, but was heavily influenced by the syntax of configuration files
used in the Asterisk PBX. Portions of the C code, specifically its lexical
scanner and shift-reduce parser, are generated using the Flex (Lex) and
Bison (Yacc) tools. Assay is built on top of the Diminuto library of
GNU/Linux-based software tools and makes heavy use of its balanced
Red-Black tree implementation.

Assay (and Diminuto) has been built and tested on a Pentium PC running
Ubuntu 14.04 (my build server), and an Nvidia Jetson TK1 board also running
Ubuntu 14.04 (my reference target). This latter target has the internal
Digital Aggregates code name of "Stampede", which you will see referenced
here and there.

This software is an original work of its author(s).

Information regarding Assay and Diminuto can be found here:

    http://www.diag.com/navigation/downloads/Assay.html

    http://www.diag.com/navigation/downloads/Diminuto.html

Contact:

    Chip Overclock
    Digital Aggregates Corporation
    3440 Youngfield Street, Suite 209
    Wheat Ridge CO 80033 USA
    http://www.diag.com
    mailto:coverclock@diag.com

Here is an example of what an INI file might look like.

    ; This property goes into the default section named "general".
    keyword01 = value01

    [Section1]
    keyword11=value11
    keyword12: value12
    keyword\ 13 : value 13

    #include common.ini

    [Section\ 2]

    keyword3 = \ value\t3
    keyword4=\
    123\
    456;

    #exec generated.sh

    [Section3] keyword5: value5
    [Section3] keyword6: value6

The syntax rules for the INI file format supported by Assay are pretty
simple (but the grammar is the definitive source).

1.  The characters octothorpe, equal sign, colon, semicolon, left square
    bracket, and right square bracket, are special.

2.  White space at the beginning of any line is ignored.

3.  A comment begins with a semicolon and can occur on a line by itself or
    on the same line after any other statement.

4.  The beginning of a section is declared within square brackets. The
    section name must escape special characters or white space, which then
    becomes part of the section name.

5.  Properties consist of a keyword, an equal sign or a colon, and a value.
    White space may occur on either side of the equal sign or colon.

6.  If a keyword contains special characters or white space, those characters
    must be escaped.

7.  The value starts at the first non-white space character following the
    equal sign or colon. A white space character that is the first character
    of a value must be escaped. The value continues until end of line or a
    comment. A semicolon in the value must be escaped.

8.  As a short cut, a section can be declared followed by a property on the
    same line.

9.  Statements can be extended across multiple lines by escaping the newline
    at the end, which is discarded.

10. An octothorpe as the first character of a statement signals an operation
    that interrupts the parsing of the current input stream. Every operation
    consists of an operator and an argument separated by white space.
    The two operators currently supported are include and exec.

11. The include operator temporarily redirects parsing to the file identified
    by the path name in the argument. When end of file is reached, parsing of
    the stream containing the include statement resumes.

12. The exec operator temporarily redirects parsing to the standard output
    of the shell command specified by the argument, which may include white
    space. When the shell command exits, parsing of the stream containing the
    exec statement resumes.
