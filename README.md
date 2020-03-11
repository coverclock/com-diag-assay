com-diag-assay
==============

[![Say Thanks!](https://img.shields.io/badge/Say%20Thanks-!-1EAEDB.svg)](https://saythanks.io/to/coverclock)

Copyright 2015-2020 by the Digital Aggregates Corporation, Colorado, USA.

# License

License under the terms in LICENSE.txt.

# Abstract

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
Ubuntu 14.04 (my build server), and an Nvidia Jetson TK1 board (which has a
64-bit ARM processor) also running Ubuntu 14.04 (my reference target). This
latter target has the internal Digital Aggregates code name of "Stampede",
which you will see referenced here and there. Both targets used Bison 3.0.2
and Flex 2.5.35; Assay is unlikely to build with earlier versions of Bison.

UPDATE 2020-02-27: Remarkably, Assay will not build under Bison 3.3.2
and Flex 2.6.4 which are the versions installed by the Debian packages
bison and flex for Raspbian 10 "buster" for the Raspberry Pi. Flex's
libfl on that ARMv7 platform seems to be compiled to expect an actual
yylex function; yylex is a preprocessor definition in the generated
code. Still works fine on an x86_64 platform running Ubuntu 18.04 "bionic"
with Bison 3.0.4 and Flex 2.6.4. I fixed this on the Pi by editing
the host.mk file to statically link to the bison and flex libraries;
the change is already present in that file as a comment.  You can also
set TARGET=buster (UNTESTED) in the Makefile, but that's not an option
for me since it requires the Diminuto library to be similarly linked
against the buster target, and I'm using Diminuto elsewhere on the Pi.

This software is an original work of its author(s).

# Contact

    Chip Overclock
    Digital Aggregates Corporation
    3440 Youngfield Street, Suite 209
    Wheat Ridge CO 80033 USA
    http://www.diag.com
    mailto:coverclock@diag.com

# Dependencies

<https://github.com/coverclock/com-diag-assay>

<https://github.com/coverclock/com-diag-diminuto>

    sudo apt-get install bison
    sudo apt-get install flex

# Build

    sudo apt-get install bison
    sudo apt-get install flex

    cd ~
    mkdir -p src
    cd src
    git clone https://github.com/coverclock/com-diag-diminuto
    cd com-diag-diminuto/Diminuto
    make pristine
    make depend
    make all

    cd ~
    mkdir -p src
    cd src
    git clone https://github.com/coverclock/com-diag-assay
    cd com-diag-assay/Assay
    ${EDITOR} cfg/host.mk
    make pristine
    make depend
    make all

# Unit Tests

Preceed with . out/host/bin/setup to setup PATH etc.

* make sanity - These tests will run in just a minute or so.

# Remarks

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

# Articles

Chip Overclock, "Configuration Files Are Just Another Form Of Message Passing",
2015-03,
<https://coverclock.blogspot.com/2015/03/configuration-files-are-just-another.html>

Chip Overclock, "When Learning By Doing Goes To Eleven", 2020-03,
<https://coverclock.blogspot.com/2020/03/when-learning-by-doing-goes-to-eleven.html>

# Soundtrack

<https://www.youtube.com/playlist?list=PLd7Yo1333iA-YIyldvOB56QS-HZed_4g1>
