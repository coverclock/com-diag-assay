# Copyright 2015 Digital Aggregates Corporation
# Licensed under the terms in README.h
# author:Chip Overclock
# mailto:coverclock@diag.com
# http://www.diag.com/navigation/downloads/Assay.html
# "Chip Overclock" is a registered trademark.
# "Digital Aggregates Corporation" is a registered trademark.
#
#	make all TARGET=<target>		# Build.
#

########## Customizations

TITLE				:=	Assay

TARGET				:=	host# Build for the x86_64 running Ubuntu 14.04 LTS.
#TARGET				:=	stampede# Build for the Nvidia Jetson TK1 running Ubuntu 14.04.

MAJOR				:=	1# API changes that may require that applications be modified.
MINOR				:=	5# Only functionality or features added with no legacy API changes.
BUILD				:=	0# Only bugs fixed with no API changes or new functionality.

VINTAGE				:=	$(shell date -u +%Y-%m-%dT%H:%M:%S.%N%z)

# This stuff all gets embedded in the vintage application.
COPYRIGHT			:=	2015 Digital Aggregates Corporation
LICENSE				:=	GNU Lesser General Public License 2.1
CONTACT				:=	coverclock@diag.com
HOMEPAGE			:=	http://www.diag.com/navigation/downloads/$(TITLE).html

# This is where I store collateral associated with projects that I have
# downloaded off the web and use without alteration. Examples: Linux kernel
# sources, toolchains, etc.
HOME_DIR			:=	$(HOME)/Projects

########## Directories

ARC_DIR				:=	arc# Archive files for static linking
BIN_DIR				:=	bin# Utility source files or stripped executable binaries
CFG_DIR				:=	cfg# Build configuration files
DEP_DIR				:=	dep# Generated dependencies and other make files
DOC_DIR				:=	doc# Documentation
ETC_DIR				:=	etc# Miscellaneous files
GEN_DIR				:=	gen# Generated source files
INC_DIR				:=	inc# Header files
LIB_DIR				:=	lib# Shared objects for dynamic linking
OUT_DIR				:=	out# Build artifacts
SRC_DIR				:=	src# Library source files
SYM_DIR				:=	sym# Unstripped executable binaries
TGZ_DIR				:=	tgz# Compressed tarballs
TST_DIR				:=	tst# Unit tests

########## Configuration

PROJECT				:=	$(shell echo $(TITLE) | tr '[A-Z]' '[a-z]')
SYMBOL				:=	$(shell echo $(TITLE) | tr '[a-z]' '[A-Z]')

HERE				:=	$(shell pwd)

OUT					:=	$(OUT_DIR)/$(TARGET)

TEMP_DIR			:=	/tmp
ROOT_DIR			:=	$(HOME_DIR)/$(TARGET)

TIMESTAMP			:=	$(shell date -u +%Y%m%d%H%M%S%N%Z)
DATESTAMP			:=	$(shell date +%Y%m%d)

SVNREPO				:=	svn://graphite/$(PROJECT)
SVNURL				:=	$(SVNREPO)/trunk/$(TITLE)
GITURL				:=	https://github.com/coverclock/com-diag-$(PROJECT).git

GENERATED			:=	vintage setup generate
SYNTHESIZED			:=	$(PROJECT)_release.h $(PROJECT)_vintage.h

ALIASES				:=	

NEW					:=	dummy
OLD					:=	dummy

PACKAGE				:=	$(OUT)/$(TGZ_DIR)/$(PROJECT)-$(TARGET)

MANIFEST			:=	$(ARC_DIR) $(BIN_DIR) $(DRV_DIR) $(ETC_DIR) $(INC_DIR) $(LIB_DIR) $(MOD_DIR) $(TST_DIR)

DISTRIBUTION		:=	$(OUT)/$(TGZ_DIR)/$(PROJECT)-$(MAJOR).$(MINOR).$(BUILD)

TARBALL				:=	$(OUT)/$(TGZ_DIR)/$(PROJECT)

SO					:=	so

A2SOARCH			:=	-shared -Wl,-soname,lib$(PROJECT).$(SO).$(MAJOR).$(MINOR).$(BUILD)

include $(CFG_DIR)/$(TARGET).mk

########## Diminuto Configuration

DIMINUTO_TITLE		:=	Diminuto

DIMINUTO_PROJECT	:=	$(shell echo $(DIMINUTO_TITLE) | tr '[A-Z]' '[a-z]')
DIMINUTO_SYMBOL		:=	$(shell echo $(DIMINUTO_TITLE) | tr '[a-z]' '[A-Z]')

DIMINUTO_ROOT		:=	$(HOME)/src/$(DIMINUTO_TITLE)/$(DIMINUTO_TITLE)
DIMINUTO_HEADERS	:=	$(DIMINUTO_ROOT)/$(INC_DIR)
DIMINUTO_LIBRARIES	:=	$(DIMINUTO_ROOT)/$(OUT)/$(LIB_DIR)

DIMINUTO_CPPFLAGS	:=	-iquote $(DIMINUTO_HEADERS)
DIMINUTO_LDFLAGS	:=	-L$(DIMINUTO_LIBRARIES) -l$(DIMINUTO_PROJECT) -lpthread -lrt -ldl -lm

########## Commands and Option Flags

PROJECT_A			:=	lib$(PROJECT).a
PROJECT_SO			:=	lib$(PROJECT).$(SO)

CROSS_COMPILE		:=	$(TOOLCHAIN)

CC					:=	$(CROSS_COMPILE)gcc
AR					:=	$(CROSS_COMPILE)ar
RANLIB				:=	$(CROSS_COMPILE)ranlib
STRIP				:=	$(CROSS_COMPILE)strip

CDEFINES			:=

ARFLAGS				:=	crsv
#CPPFLAGS			:=	$(CDEFINES) -I $(SRC_DIR) -I $(INC_DIR) -I $(OUT)/$(INC_DIR) $(CPPARCH)
CPPFLAGS			:=	$(CDEFINES) -iquote $(OUT)/$(GEN_DIR) -iquote $(SRC_DIR) -iquote $(INC_DIR) -iquote $(OUT)/$(INC_DIR) $(DIMINUTO_CPPFLAGS) $(CPPARCH)
CFLAGS				:=	$(CARCH) -g
#CFLAGS				:=	$(CARCH) -g -fprofile-arcs -ftest-coverage
#CFLAGS				:=	$(CARCH) -O3
CPFLAGS				:=	-i
MVFLAGS				:=	-i
LDFLAGS				:=	$(LDARCH) -l$(PROJECT) $(LDLIBRARIES) $(DIMINUTO_LDFLAGS)
MOFLAGS				:=	$(MOARCH) -l$(PROJECT) $(LDLIBRARIES)
SOFLAGS				:=	$(SOARCH) $(LDLIBRARIES)

BROWSER				:=	firefox

########## Build Artifacts

TARGETOBJECTS		:=	$(addprefix $(OUT)/,$(addsuffix .o,$(basename $(wildcard $(SRC_DIR)/*.c))))
TARGETOBJECTS		+=	$(addprefix $(OUT)/,$(addsuffix .o,$(basename $(wildcard $(SRC_DIR)/*.l))))
TARGETOBJECTS		+=	$(addprefix $(OUT)/,$(addsuffix .o,$(basename $(wildcard $(SRC_DIR)/*.y))))
TARGETSCRIPTS		:=	$(addprefix $(OUT)/,$(basename $(wildcard $(BIN_DIR)/*.sh)))
TARGETBINARIES		:=	$(addprefix $(OUT)/,$(basename $(wildcard $(BIN_DIR)/*.c)))
TARGETGENERATED		:=	$(addprefix $(OUT)/$(BIN_DIR)/,$(GENERATED))
TARGETSYNTHESIZED	:=	$(addprefix $(OUT)/$(INC_DIR)/com/diag/$(PROJECT)/,$(SYNTHESIZED))

TARGETALIASES		:=	$(addprefix $(OUT)/$(BIN_DIR)/,$(ALIASES))
TARGETUNITTESTS		:=	$(addprefix $(OUT)/,$(basename $(wildcard $(TST_DIR)/*.c)))
TARGETUNITTESTS		+=	$(addprefix $(OUT)/,$(basename $(wildcard $(TST_DIR)/*.sh)))

TARGETARCHIVE		:=	$(OUT)/$(ARC_DIR)/$(PROJECT_A)
TARGETSHARED		:=	$(OUT)/$(LIB_DIR)/$(PROJECT_SO).$(MAJOR).$(MINOR).$(BUILD)
TARGETSHARED		+=	$(OUT)/$(LIB_DIR)/$(PROJECT_SO).$(MAJOR).$(MINOR)
TARGETSHARED		+=	$(OUT)/$(LIB_DIR)/$(PROJECT_SO).$(MAJOR)
TARGETSHARED		+=	$(OUT)/$(LIB_DIR)/$(PROJECT_SO)

TARGETLIBRARIES		:=	$(TARGETARCHIVE) $(TARGETSHARED)
TARGETPROGRAMS		:=	$(TARGETBINARIES) $(TARGETALIASES) $(TARGETUNITTESTS) $(TARGETGENERATED) $(TARGETSCRIPTS)
TARGETDEFAULT		:=	$(TARGETLIBRARIES) $(TARGETPROGRAMS)
TARGETMOST			:=	$(TARGETDEFAULT)
TARGETALL			:=	$(TARGETMOST)

########## Main Entry Points

.PHONY:	default all dist clean pristine clobber

default:	$(TARGETDEFAULT)

most:	$(TARGETMOST)

all:	$(TARGETALL)

dist:	distribution

clean:
	rm -rf $(OUT)
	rm -rf *.gcda *.gcno *.gcov

pristine clobber:	clean
	rm -rf $(OUT_DIR)

########## Lovingly Hand Crafted Targets With Dependencies Generated At Build

$(OUT)/$(SRC_DIR)/$(PROJECT)_parser.o:	$(OUT)/$(GEN_DIR)/$(PROJECT)_parser.c

$(OUT)/$(GEN_DIR)/$(PROJECT)_parser.h:	$(OUT)/$(GEN_DIR)/$(PROJECT)_parser.c

$(OUT)/$(SRC_DIR)/$(PROJECT)_scanner.o:	$(OUT)/$(GEN_DIR)/$(PROJECT)_scanner.c $(OUT)/$(GEN_DIR)/$(PROJECT)_parser.h

$(OUT)/$(GEN_DIR)/$(PROJECT)_scanner.h:	$(OUT)/$(GEN_DIR)/$(PROJECT)_scanner.c

$(OUT)/$(SRC_DIR)/$(PROJECT)_parser_annex.o:	$(OUT)/$(GEN_DIR)/$(PROJECT)_parser.h $(OUT)/$(GEN_DIR)/$(PROJECT)_scanner.h

$(OUT)/$(SRC_DIR)/$(PROJECT)_scanner_annex.o:	$(OUT)/$(GEN_DIR)/$(PROJECT)_parser.h $(OUT)/$(GEN_DIR)/$(PROJECT)_scanner.h

$(OUT)/$(SRC_DIR)/$(PROJECT).o:	$(OUT)/$(GEN_DIR)/$(PROJECT)_parser.h $(OUT)/$(GEN_DIR)/$(PROJECT)_scanner.h

########## Packaging and Distribution

# Useful for copying the executables over to another target for which they were
# cross compiled.

.PHONY:	package

package $(PACKAGE).tgz:
	D=`dirname $(PACKAGE)`; mkdir -p $$D; \
	for M in $(MANIFEST); do mkdir -p $(OUT)/$$M; done; \
	T=`mktemp -d "$(TEMP_DIR)/$(PROJECT).XXXXXXXXXX"`; \
	B=`basename $(PACKAGE)`; mkdir -p $$T/$$B; \
	tar -C $(OUT) --exclude-vcs -cvf - $(MANIFEST) | tar -C $$T/$$B -xvf -; \
	tar --exclude-vcs -cvf - $(INC_DIR) $(ETC_DIR) | tar -C $$T/$$B -xvf -; \
	tar -C $$T --exclude-vcs -cvzf - $$B > $(PACKAGE).tgz; \
	rm -rf $$T

# Useful for given someone a tarball of the actual source distribution that
# is guaranteed to at least build.

.PHONY:	distribution distro

distribution distro $(DISTRIBUTION).tgz:
	D=`dirname $(DISTRIBUTION)`; mkdir -p $$D; \
	T=`mktemp -d "$(TEMP_DIR)/$(PROJECT).XXXXXXXXXX"`; \
	B=`basename $(DISTRIBUTION)`; \
	( cd $$T; svn export $(SVNURL) $$B; ); \
	tar -C $$T --exclude-vcs -cvzf - $$B > $(DISTRIBUTION).tgz; \
	rm -rf $$T/$$B; \
	tar -C $$T -xvzf - < $(DISTRIBUTION).tgz; \
	( cd $$T/$$B; make all TARGET=host OUT=out/host && $(OUT)/bin/generate > ./setup && . ./setup && vintage ); \
	rm -rf $$T

# Useful for copying a tarball of the current development source base to a
# target for which there is no cross compiler toolchain.

.PHONY:	tarball

tarball $(TARBALL).tgz:
	D=`dirname $(TARBALL)`; mkdir -p $$D; \
	T=`pwd`; \
	B=`basename $$T`; \
	( tar -C .. --exclude-vcs --exclude=$(OUT_DIR) --exclude=.??* -cvzf - $$B ) > $(TARBALL).tgz; \

# Useful for backing a complete backup before doing something that may turn
# out to be profoundly stupid, like running a transformative script across all
# the source files. Sure, your local and remote repos should save you, but what
# if they didn't?

.PHONY:	backup

backup:
	tar --exclude=$(OUT_DIR) --exclude-vcs -cvzf - . > ../../$(PROJECT)-$(TIMESTAMP).tgz

########## Source Code Control

.PHONY: commit dcommit push origin init

commit:
	git commit .

dcommit:
	git svn dcommit

push:
	git push origin master

pull:
	git pull origin master

init:
	git svn show-ignore > .gitignore
	git add .gitignore
	git remote add origin $(GITURL)

########## Target C Libraries

$(OUT)/$(ARC_DIR)/$(PROJECT_A):	$(TARGETOBJECTS)
	D=`dirname $@`; mkdir -p $$D
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO).$(MAJOR).$(MINOR).$(BUILD):	$(TARGETOBJECTS)
	D=`dirname $@`; mkdir -p $$D
	$(CC) $(CFLAGS) $(A2SOARCH) -o $@ $(SOFLAGS) $(TARGETOBJECTS)

$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO):	$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO).$(MAJOR).$(MINOR).$(BUILD)
	D=`dirname $<`; F=`basename $<`; T=`basename $@`; ( cd $$D; ln -s -f $$F $$T ) 

$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO).$(MAJOR):	$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO).$(MAJOR).$(MINOR).$(BUILD)
	D=`dirname $<`; F=`basename $<`; T=`basename $@`; ( cd $$D; ln -s -f $$F $$T ) 

$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO).$(MAJOR).$(MINOR):	$(OUT)/$(LIB_DIR)/lib$(PROJECT).$(SO).$(MAJOR).$(MINOR).$(BUILD)
	D=`dirname $<`; F=`basename $<`; T=`basename $@`; ( cd $$D; ln -s -f $$F $$T ) 

########## Target Unstripped Binaries

$(OUT)/$(SYM_DIR)/%:	$(BIN_DIR)/%.c $(TARGETLIBRARIES)
	D=`dirname $@`; mkdir -p $$D
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

########## Target Aliases

$(OUT)/$(BIN_DIR)/hex $(OUT)/$(BIN_DIR)/oct $(OUT)/$(BIN_DIR)/ntohs $(OUT)/$(BIN_DIR)/htons $(OUT)/$(BIN_DIR)/ntohl $(OUT)/$(BIN_DIR)/htonl:	$(OUT)/$(BIN_DIR)/dec
	ln -f $< $@

########## Unit Tests

$(OUT)/$(TST_DIR)/%:	$(TST_DIR)/%.c $(TARGETLIBRARIES)
	D=`dirname $@`; mkdir -p $$D
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

########## Generated Source Files

ifeq (3.82, $(firstword $(sort $(MAKE_VERSION) 3.82)))

$(OUT)/$(GEN_DIR)/vintage.c:	$(TARGETSYNTHESIZED) Makefile
	@echo MAKE_VERSION=$(MAKE_VERSION)
	D=`dirname $@`; mkdir -p $$D	
	echo '/* GENERATED FILE! DO NOT EDIT! */' > $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_release.h"' >> $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_release.h"' >> $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_vintage.h"' >> $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_vintage.h"' >> $@
	echo '#include <stdio.h>' >> $@
	echo '#include <assert.h>' >> $@
	echo 'static const char METADATA[] =' >> $@
	echo '    "Title = $(TITLE)\n"' >> $@
	echo '    "Copyright = $(COPYRIGHT)\n"' >> $@
	echo '    "Contact = $(CONTACT)\n"' >> $@
	echo '    "License = $(LICENSE)\n"' >> $@
	echo '    "Homepage = $(HOMEPAGE)\n"' >> $@
	echo '    "Release = " COM_DIAG_$(SYMBOL)_RELEASE "\n"' >> $@
	echo '    "Vintage = " COM_DIAG_$(SYMBOL)_VINTAGE "\n"' >> $@
	echo '    "Host = $(shell hostname)\n"' >> $@
	echo '    "Directory = $(shell pwd)\n"' >> $@
	echo '    "Arch = $(ARCH)\n"' >> $@
	echo '    "Target = $(TARGET)\n"' >> $@
	echo '    "Os = $(OS)\n"' >> $@
	echo '    "Toolchain = $(TOOLCHAIN)\n"' >> $@
	echo '    "User = $(USER)\n"' >> $@
	echo ';' >> $@
	echo 'extern const char COM_DIAG_$(SYMBOL)_RELEASE_KEYWORD[];' >> $@
	echo 'extern const char * COM_DIAG_$(SYMBOL)_RELEASE_VALUE;' >> $@
	echo 'extern const char COM_DIAG_$(SYMBOL)_VINTAGE_KEYWORD[];' >> $@
	echo 'extern const char * COM_DIAG_$(SYMBOL)_VINTAGE_VALUE;' >> $@
	echo 'int main(void) {' >> $@
	echo '    const char * release_keyword = (const char *)0;' >> $@
	echo '    const char * release_value = (const char *)0;' >> $@
	echo '    const char * vintage_keyword = (const char *)0;' >> $@
	echo '    const char * vintage_value = (const char *)0;' >> $@
	echo '    fputs(METADATA, stderr);' >> $@
	echo '    fputs("$(MAJOR).$(MINOR).$(BUILD)\n", stdout);' >> $@
	echo '    release_keyword = COM_DIAG_$(SYMBOL)_RELEASE_KEYWORD;' >> $@
	echo '    release_value = COM_DIAG_$(SYMBOL)_RELEASE_VALUE;' >> $@
	echo '    vintage_keyword = COM_DIAG_$(SYMBOL)_VINTAGE_KEYWORD;' >> $@
	echo '    vintage_value = COM_DIAG_$(SYMBOL)_VINTAGE_VALUE;' >> $@
	echo '    assert(release_keyword != (const char *)0);' >> $@
	echo '    assert(release_value != (const char *)0);' >> $@
	echo '    assert(vintage_keyword != (const char *)0);' >> $@
	echo '    assert(vintage_value != (const char *)0);' >> $@
	echo '    return 0;' >> $@
	echo '}' >> $@

else

$(OUT)/$(GEN_DIR)/vintage.c:	$(TARGETSYNTHESIZED) Makefile
	@echo MAKE_VERSION=$(MAKE_VERSION)
	D=`dirname $@`; mkdir -p $$D	
	echo '/* GENERATED FILE! DO NOT EDIT! */' > $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_release.h"' >> $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_release.h"' >> $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_vintage.h"' >> $@
	echo '#include "com/diag/$(PROJECT)/$(PROJECT)_vintage.h"' >> $@
	echo '#include <stdio.h>' >> $@
	echo '#include <assert.h>' >> $@
	echo 'static const char METADATA[] =' >> $@
	echo '    "Title = $(TITLE)\\n"' >> $@
	echo '    "Copyright = $(COPYRIGHT)\\n"' >> $@
	echo '    "Contact = $(CONTACT)\\n"' >> $@
	echo '    "License = $(LICENSE)\\n"' >> $@
	echo '    "Homepage = $(HOMEPAGE)\\n"' >> $@
	echo '    "Release = " COM_DIAG_$(SYMBOL)_RELEASE "\\n"' >> $@
	echo '    "Vintage = " COM_DIAG_$(SYMBOL)_VINTAGE "\\n"' >> $@
	echo '    "Host = $(shell hostname)\\n"' >> $@
	echo '    "Directory = $(shell pwd)\\n"' >> $@
	echo '    "Arch = $(ARCH)\\n"' >> $@
	echo '    "Target = $(TARGET)\\n"' >> $@
	echo '    "Os = $(OS)\\n"' >> $@
	echo '    "Toolchain = $(TOOLCHAIN)\\n"' >> $@
	echo '    "User = $(USER)\\n"' >> $@
	echo ';' >> $@
	echo 'extern const char COM_DIAG_$(SYMBOL)_RELEASE_KEYWORD[];' >> $@
	echo 'extern const char * COM_DIAG_$(SYMBOL)_RELEASE_VALUE;' >> $@
	echo 'extern const char COM_DIAG_$(SYMBOL)_VINTAGE_KEYWORD[];' >> $@
	echo 'extern const char * COM_DIAG_$(SYMBOL)_VINTAGE_VALUE;' >> $@
	echo 'int main(void) {' >> $@
	echo '    const char * release_keyword = (const char *)0;' >> $@
	echo '    const char * release_value = (const char *)0;' >> $@
	echo '    const char * vintage_keyword = (const char *)0;' >> $@
	echo '    const char * vintage_value = (const char *)0;' >> $@
	echo '    fputs(METADATA, stderr);' >> $@
	echo '    fputs("$(MAJOR).$(MINOR).$(BUILD)\\n", stdout);' >> $@
	echo '    release_keyword = COM_DIAG_$(SYMBOL)_RELEASE_KEYWORD;' >> $@
	echo '    release_value = COM_DIAG_$(SYMBOL)_RELEASE_VALUE;' >> $@
	echo '    vintage_keyword = COM_DIAG_$(SYMBOL)_VINTAGE_KEYWORD;' >> $@
	echo '    vintage_value = COM_DIAG_$(SYMBOL)_VINTAGE_VALUE;' >> $@
	echo '    assert(release_keyword != (const char *)0);' >> $@
	echo '    assert(release_value != (const char *)0);' >> $@
	echo '    assert(vintage_keyword != (const char *)0);' >> $@
	echo '    assert(vintage_value != (const char *)0);' >> $@
	echo '    return 0;' >> $@
	echo '}' >> $@

endif

# For embedding in an application where it can be interrogated or displayed.
$(OUT)/$(INC_DIR)/com/diag/$(PROJECT)/$(PROJECT)_release.h:	Makefile
	D=`dirname $@`; mkdir -p $$D	
	echo '/* GENERATED FILE! DO NOT EDIT! */' > $@
	echo '#ifndef _H_COM_DIAG_$(SYMBOL)_RELEASE_' >> $@
	echo '#define _H_COM_DIAG_$(SYMBOL)_RELEASE_' >> $@
	echo '#define COM_DIAG_$(SYMBOL)_RELEASE_MAJOR $(MAJOR)' >> $@
	echo '#define COM_DIAG_$(SYMBOL)_RELEASE_MINOR $(MINOR)' >> $@
	echo '#define COM_DIAG_$(SYMBOL)_RELEASE_BUILD $(BUILD)' >> $@
	echo '#define COM_DIAG_$(SYMBOL)_RELEASE "$(MAJOR).$(MINOR).$(BUILD)"' >> $@
	echo '#endif' >> $@

# For embedding in the library archive and shared object.
$(OUT)/$(SRC_DIR)/$(PROJECT)_release.o:	$(TARGETSYNTHESIZED)

# For embedding in an application where it can be interrogated or displayed.
$(OUT)/$(INC_DIR)/com/diag/$(PROJECT)/$(PROJECT)_vintage.h:	Makefile
	D=`dirname $@`; mkdir -p $$D	
	echo '/* GENERATED FILE! DO NOT EDIT! */' > $@
	echo '#ifndef _H_COM_DIAG_$(SYMBOL)_VINTAGE_' >> $@
	echo '#define _H_COM_DIAG_$(SYMBOL)_VINTAGE_' >> $@
	echo '#define COM_DIAG_$(SYMBOL)_VINTAGE "$(VINTAGE)"' >> $@
	echo '#endif' >> $@

# For embedding in the library archive and shared object.
$(OUT)/$(SRC_DIR)/$(PROJECT)_vintage.o:	$(TARGETSYNTHESIZED)

# For executing from the command line during testing.
$(OUT)/$(SYM_DIR)/vintage:	$(OUT)/$(GEN_DIR)/vintage.c
	D=`dirname $@`; mkdir -p $$D	
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

# For sourcing into a bash shell (for example, ". setup").
$(OUT)/$(BIN_DIR)/setup:	Makefile
	D=`dirname $@`; mkdir -p $$D	
	echo 'COM_DIAG_$(SYMBOL)_PATH=`dirname $${BASH_ARGV[0]}`; COM_DIAG_$(SYMBOL)_ROOT=`cd $$COM_DIAG_$(SYMBOL)_PATH; pwd`' > $@
	echo 'export PATH=$$PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../sym:$$COM_DIAG_$(SYMBOL)_ROOT/../bin:$$COM_DIAG_$(SYMBOL)_ROOT/../tst' >> $@
	echo 'export LD_DRIVER_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../drv' >> $@
	echo 'export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../lib' >> $@
	echo 'export LD_MODULE_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../mod' >> $@
	echo 'export COM_DIAG_DIMINUTO_LOG_MASK=0xffff' >> $@
	chmod 644 $@

$(OUT)/$(SYM_DIR)/setup:	Makefile
	D=`dirname $@`; mkdir -p $$D	
	echo 'COM_DIAG_$(SYMBOL)_PATH=`dirname $${BASH_ARGV[0]}`; COM_DIAG_$(SYMBOL)_ROOT=`cd $$COM_DIAG_$(SYMBOL)_PATH; pwd`' > $@
	echo 'export PATH=$$PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../sym:$$COM_DIAG_$(SYMBOL)_ROOT/../tst' >> $@
	echo 'export LD_DRIVER_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../drv' >> $@
	echo 'export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../lib' >> $@
	echo 'export LD_MODULE_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../mod' >> $@
	echo 'export COM_DIAG_DIMINUTO_LOG_MASK=0xffff' >> $@
	chmod 644 $@

# For generating a setup script for a bash shell (for example, "bash generate > setup").
# (Because the Android bash doesn't seem to implement BASH_ARGV.)
$(OUT)/$(BIN_DIR)/generate:	Makefile
	D=`dirname $@`; mkdir -p $$D	
	echo 'COM_DIAG_$(SYMBOL)_PATH=`dirname $$0`; COM_DIAG_$(SYMBOL)_ROOT=`cd $$COM_DIAG_$(SYMBOL)_PATH; pwd`' > $@
	echo 'echo export PATH=$$PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../sym:$$COM_DIAG_$(SYMBOL)_ROOT/../bin:$$COM_DIAG_$(SYMBOL)_ROOT/../tst' >> $@
	echo 'echo export LD_DRIVER_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../drv' >> $@
	echo 'echo export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../lib' >> $@
	echo 'echo export LD_MODULE_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../mod' >> $@
	echo 'echo export COM_DIAG_DIMINUTO_LOG_MASK=0xffff' >> $@
	chmod 755 $@

$(OUT)/$(SYM_DIR)/generate:	Makefile
	D=`dirname $@`; mkdir -p $$D	
	echo 'COM_DIAG_$(SYMBOL)_PATH=`dirname $$0`; COM_DIAG_$(SYMBOL)_ROOT=`cd $$COM_DIAG_$(SYMBOL)_PATH; pwd`' > $@
	echo 'echo export PATH=$$PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../sym:$$COM_DIAG_$(SYMBOL)_ROOT/../tst' >> $@
	echo 'echo export LD_DRIVER_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../drv' >> $@
	echo 'echo export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$$COM_DIAG_$(SYMBOL)_ROOT/../lib' >> $@
	echo 'echo export LD_MODULE_PATH=$$COM_DIAG_$(SYMBOL)_ROOT/../mod' >> $@
	echo 'echo export COM_DIAG_DIMINUTO_LOG_MASK=0xffff' >> $@
	chmod 755 $@

########## Helpers

.PHONY:	makeversion gccversion implicit defines iquotes isystems

makeversion:
	@echo $(MAKE_VERSION)

gccversion:
	@$(CC) -x c $(CPPFLAGS) $(CFLAGS) -E -v - < /dev/null

implicit:
	@$(CC) $(CFLAGS) -dM -E - < /dev/null
	
defines:
	@$(CC) $(CPPFLAGS) $(CFLAGS) -dM -E - < /dev/null

iquotes:
	@$(CC) -x c $(CPPFLAGS) $(CFLAGS) -E -v - < /dev/null 2>&1 | awk 'BEGIN { S=0; } ($$0=="#include \"...\" search starts here:") { S=1; next; } ($$0=="#include <...> search starts here:") { S=0; next; } (S!=0) { print $$1; } { next; }'

isystems:
	@$(CC) -x c $(CPPFLAGS) $(CFLAGS) -E -v - < /dev/null 2>&1 | awk 'BEGIN { S=0; } ($$0=="#include <...> search starts here:") { S=1; next; } ($$0=="End of search list.") { S=0; next; } (S!=0) { print $$1; } { next; }'

########## Documentation

.PHONY:	documentation browse refman manpages

DOCCOOKED := $(shell echo $(OUT)/$(DOC_DIR) | sed 's/\//\\\//g')

documentation $(OUT)/$(DOC_DIR)/latex $(OUT)/$(DOC_DIR)/man $(OUT)/$(DOC_DIR)/pdf:
	mkdir -p $(OUT)/$(DOC_DIR)/pdf
	cat doxygen.cf | \
		sed -e "s/\\\$$PROJECT_NUMBER\\\$$/$(MAJOR).$(MINOR).$(BUILD)/" | \
		sed -e "s/\\\$$OUTPUT_DIRECTORY\\\$$/$(DOCCOOKED)/" | \
		cat > $(OUT)/$(DOC_DIR)/doxygen-local.cf
	doxygen $(OUT)/$(DOC_DIR)/doxygen-local.cf

# This appears to be broken in Doxygen 1.8.6. in Ubuntu 14.04. Used to work.
documentation-ancillary:	$(OUT)/$(DOC_DIR)/latex $(OUT)/$(DOC_DIR)/man $(OUT)/$(DOC_DIR)/pdf
	$(MAKE) -C $(OUT)/$(DOC_DIR)/latex refman.pdf
	cp $(OUT)/$(DOC_DIR)/latex/refman.pdf $(OUT)/$(DOC_DIR)/pdf
	cat $(OUT)/$(DOC_DIR)/man/man3/*.3 | groff -man -Tps - > $(OUT)/$(DOC_DIR)/pdf/manpages.ps
	ps2pdf $(OUT)/$(DOC_DIR)/pdf/manpages.ps $(OUT)/$(DOC_DIR)/pdf/manpages.pdf

browse:
	$(BROWSER) file:$(OUT)/$(DOC_DIR)/html/index.html

refman:
	$(BROWSER) file:$(OUT)/$(DOC_DIR)/pdf/refman.pdf

manpages:
	$(BROWSER) file:$(OUT)/$(DOC_DIR)/pdf/manpages.pdf

########## Diffs and Patches

.PHONY:	patch

patch:	$(OLD) $(NEW)
	diff -purN $(OLD) $(NEW)

########## Rules

$(OUT)/%.txt:	%.c
	D=`dirname $@`; mkdir -p $$D
	$(CC) -E $(CPPFLAGS) -c $< > $@

$(OUT)/%.o:	%.c
	D=`dirname $@`; mkdir -p $$D
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

$(OUT)/%:	%.sh
	D=`dirname $@`; mkdir -p $$D
	cp $< $@
	chmod 755 $@

$(OUT)/$(GEN_DIR)/%.c:	$(SRC_DIR)/%.y
	D=`dirname $@`; mkdir -p $$D
	bison -v -o $@ --defines=$(addsuffix .h,$(basename $@)) $<

$(OUT)/$(GEN_DIR)/%.c:	$(SRC_DIR)/%.l
	D=`dirname $@`; mkdir -p $$D
	flex --always-interactive -o $@ --header-file=$(addsuffix .h,$(basename $@)) $<

$(OUT)/$(SRC_DIR)/%.o:	$(OUT)/$(GEN_DIR)/%.c
	D=`dirname $@`; mkdir -p $$D
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.SECONDARY:

$(OUT)/$(BIN_DIR)/%:	$(OUT)/$(SYM_DIR)/%
	D=`dirname $@`; mkdir -p $$D
	$(STRIP) -o $@ $<

########## Dependencies

.PHONY:	depend

DEPENDENCIES := $(OUT)/$(DEP_DIR)/dependencies.mk

depend:	$(TARGETSYNTHESIZED)
	M=`dirname $(DEPENDENCIES)`; mkdir -p $$M
	cp /dev/null $(DEPENDENCIES)
	for S in $(BIN_DIR) $(SRC_DIR) $(TST_DIR); do \
		if [ -d $$S ]; then \
			for F in $$S/*.c; do \
				D=`dirname $$F`; \
				T=`mktemp "$(TEMP_DIR)/$(PROJECT).XXXXXXXXXX"`; \
				echo -n "$(OUT)/$$D/" > $$T; \
				$(CC) $(CPPFLAGS) -MM -MG $$F >> $$T && cat $$T >> $(DEPENDENCIES); \
				rm -f $$T; \
			done; \
		fi; \
	done

-include $(DEPENDENCIES)
