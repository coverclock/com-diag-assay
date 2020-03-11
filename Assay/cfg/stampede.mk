# Copyright 2008-2015 Digital Aggregates Corporation
# Licensed under the terms in LICENSE.txtz
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
LDLIBRARIES			:=	-lfl# -ly

# The Assay unit tests all pass when run on Stampede. I was too lazy to cross-
# build the libfl Flex library (the liby Bison library isn't needed) for the
# Stampede target, so instead I use used the regular apt-get tool on the Jetson
# board itself to download and install the libfl.so and libfl_pic.a libraries on
# the board, then I copied those files to the appropriate directory for the
# cross-compile tool chain on my build server (for me, that was
# /usr/lib/gcc-cross/arm-linux-gnueabihf/4.8), and finally edited libfl.so (it's
# a text file containing linker commands) to point to the libfl_pic.a file in
# that directory. It was a lot easier than it may sound. Then I was able to
# cross-build Assay (and of course Diminuto) for the Jetson board. It is also
# easy to natively build Assay and Diminuto on the Jetson board itself.

