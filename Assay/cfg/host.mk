# Copyright 2015 Digital Aggregates Corporation
# Licensed under the terms in README.h
# author:Chip Overclock
# mailto:coverclock@diag.com
# http://www.diag.com/navigation/downloads/Assay.html
# "Chip Overclock" is a registered trademark.
# "Digital Aggregates Corporation" is a registered trademark.

# host: Dell Inspiron 530 x86_64 running Ubuntu 14.04 LTS.

ARCH				:=	x86_64
OS					:=	linux
TOOLCHAIN			:=
CPPARCH				:=	
CARCH				:=	-rdynamic -fPIC
LDARCH				:=	-L$(OUT)/$(LIB_DIR)
MOARCH				:=	-L$(OUT)/$(LIB_DIR)
SOARCH				:=
KERNELARCH			:=
LDLIBRARIES			:=	-lfl -ly
