# vi: set ts=4 shiftwidth=4:
# Copyright 2015-2020 Digital Aggregates Corporation
# Licensed under the terms in LICENSE.txt
# author:Chip Overclock
# mailto:coverclock@diag.com
# http://www.diag.com/navigation/downloads/Assay.html
# "Chip Overclock" is a registered trademark.
# "Digital Aggregates Corporation" is a registered trademark.

ARCH				:=	armv7l
OS					:=	linux
TOOLCHAIN			:=
CPPARCH				:=	
CARCH				:=	-rdynamic -fPIC
LDARCH				:=	-L$(OUT)/$(LIB_DIR)
MOARCH				:=	-L$(OUT)/$(LIB_DIR)
SOARCH				:=
KERNELARCH			:=
LDLIBRARIES			:=	/usr/lib/arm-linux-gnueabihf/libfl.a /usr/lib/arm-linux-gnueabihf/liby.a
