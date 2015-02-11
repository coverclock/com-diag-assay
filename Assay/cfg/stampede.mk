# Copyright 2015 Digital Aggregates Corporation
# Licensed under the terms in README.h
# author:Chip Overclock
# mailto:coverclock@diag.com
# http://www.diag.com/navigation/downloads/Diminuto.html
# "Chip Overclock" is a registered trademark.
# "Digital Aggregates Corporation" is a registered trademark.

# stampede: Nvidia Jetson TK1 running Ubuntu 14.04.

ARCH				:=	arm
OS					:=	linux
TOOLCHAIN			:=	$(ARCH)-$(OS)-gnueabihf-
CPPARCH				:=	
CARCH				:=	-rdynamic -fno-omit-frame-pointer -funwind-tables -fPIC
CXXARCH				:=	$(CARCH)
LDARCH				:=	-Bdynamic -L$(OUT)/$(LIB_DIR)
MOARCH				:=	-L$(OUT)/$(LIB_DIR)
SOARCH				:=
SOXXARCH			:=	-L$(OUT)/$(LIB_DIR) -l$(PROJECT)
KERNELARCH			:=
LDLIBRARIES			:=	-lfl -ly
LDXXLIBRARIES		:=	$(LDLIBRARIES)
