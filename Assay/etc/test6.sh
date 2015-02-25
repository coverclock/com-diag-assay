#!/bin/bash
N=1
for V in $*; do
    echo KEYWORD6$N: VALUE6$V
    N=$(($N + 1))
done
exit 0
