# Copyright 2015 Digital Aggregates Corporation
# Licensed under the terms in README.hz
# author:Chip Overclock
# mailto:coverclock@diag.com
# http://www.diag.com/navigation/downloads/Assay.html
# "Chip Overclock" is a registered trademark.
# "Digital Aggregates Corporation" is a registered trademark.

# host: Dell Inspiron 530 x86_64 running Ubuntu 14.04 LTS.

ARCH				:=	x86_64
OS					:=	linux
TOOLCHAIN			:=
KERNELCHAIN			:=
KERNEL_REV			:=	3.13.0-24
KERNEL_DIR			:=	$(HOME_DIR)/host/linux-source-3.13.0
CPPARCH				:=	-isystem /usr/src/linux-headers-$(KERNEL_REV)-generic
CARCH				:=	-rdynamic -fPIC
CXXARCH				:=	$(CARCH)
LDARCH				:=	-L$(OUT)/$(LIB_DIR)
MOARCH				:=	-L$(OUT)/$(LIB_DIR)
SOARCH				:=
SOXXARCH			:=	-L$(OUT)/$(LIB_DIR) -l$(PROJECT)
KERNELARCH			:=
LDLIBRARIES			:=	-ly -lf
LDXXLIBRARIES		:=	$(LDLIBRARIES)