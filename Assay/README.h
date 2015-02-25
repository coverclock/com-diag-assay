/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_ASSAY_README_
#define _H_COM_DIAG_ASSAY_README_

/*******************************************************************************

    Copyright 2015 by the Digital Aggregates Corporation, Colorado, USA.

    ----------------------------------------------------------------------

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

	BECAUSE THE SOFTWARE IS LICENSED FREE OF CHARGE, THERE IS NO
	WARRANTY FOR THE SOFTWARE, TO THE EXTENT PERMITTED BY APPLICABLE
	LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
	HOLDERS AND/OR OTHER PARTIES PROVIDE THE SOFTWARE "AS IS" WITHOUT
	WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
	BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO THE
	QUALITY AND PERFORMANCE OF THE SOFTWARE IS WITH YOU.  SHOULD THE
	SOFTWARE PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY
	SERVICING, REPAIR OR CORRECTION.

	IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
	WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY
	MODIFY AND/OR REDISTRIBUTE THE SOFTWARE AS PERMITTED ABOVE,
	BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL,
	INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR
	INABILITY TO USE THE SOFTWARE (INCLUDING BUT NOT LIMITED TO LOSS
	OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED
	BY YOU OR THIRD PARTIES OR A FAILURE OF THE LIBRARY TO OPERATE
	WITH ANY OTHER SOFTWARE), EVEN IF SUCH HOLDER OR OTHER PARTY
	HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

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

    ----------------------------------------------------------------------

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

*******************************************************************************/

/**
 *  @file
 *
 *  This is the README for this project.
 */

#endif
