# Copyright 2008-2015 Digital Aggregates Corporation
# Licensed under the terms in README.hz
# author:Chip Overclock
# mailto:coverclock@diag.com
# http://www.diag.com/navigation/downloads/Diminuto.html
# "Chip Overclock" is a registered trademark.
# "Digital Aggregates Corporation" is a registered trademark.

# stampede: Nvidia Jetson TK1 running Ubuntu 14.04.

ARCH				:=	arm
OS					:=	linux
TOOLCHAIN			:=	$(ARCH)-$(OS)-gnueabihf-
KERNELCHAIN			:=	$(TOOLCHAIN)
KERNEL_REV			:=	3.10.24-jetson
KERNEL_DIR			:=	$(ROOT_DIR)/linux-$(KERNEL_REV)
CPPARCH				:=	-isystem $(ROOT_DIR)/linux-headers-$(KERNEL_REV)/include
CARCH				:=	-rdynamic -fno-omit-frame-pointer -funwind-tables -fPIC
LDARCH				:=	-Bdynamic -L$(OUT)/$(LIB_DIR)
MOARCH				:=	-L$(OUT)/$(LIB_DIR)
SOARCH				:=
KERNELARCH			:=
LDLIBRARIES			:=	-lfl -ly

# The Assay unit tests all pass under Stampede, both Assay and Diminuto having
# been compiled natively on the Jetson board itself. That's because I haven't
# yet tracked down the cross compiled liby and libfl Bison and Flex libraries
# I would need on my build server, and I'm too lazy at the moment to cross build
# them from scratch. But valgrind complains bitterly about a jump based on an
# undefined variable somewhere inside the dynamic loader. Not sure what to think
# about that. However, I'm confident that Assay (and Diminuto) works in at least
# this particular ARM environment. Be aware though, that, like my Pentium build
# server, this ARM target is 64-bit. So it's not as diverse a test as it might
# appear at first glance.
