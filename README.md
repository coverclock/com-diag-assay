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
