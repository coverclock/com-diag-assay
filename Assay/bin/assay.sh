#!/bin/bash
# Copyright 2016 Digital Aggregates Corporation, Colorado, USA
# Licensed under the terms in README.h
# Chip Overclock <coverclock@diag.com>
# http://www.diag.com/navigation/downloads/Assay.html
if [ $# -eq 0 ]; then
    exec echo "usage: `basename ${0}` [ SECTION ] KEYWORD" 1>&2
elif [ $# -eq 1 ]; then
    exec assaytool -f - -i -k "${1}" -r
elif [ $# -eq 2 ]; then
    exec assaytool -f - -i -s "${1}" -k "${2}" -r
else
    echo "usage: `basename ${0}` [ SECTION ] KEYWORD" 1>&2
    exit 1
fi
