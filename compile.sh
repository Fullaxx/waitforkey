#!/bin/bash

set -e

OPT="-O2"
DBG="-ggdb3 -DDEBUG"
CFLAGS="-Wall"
OPTCFLAGS="${CFLAGS} ${OPT}"
DBGCFLAGS="${CFLAGS} ${DBG}"

rm -f *.exe *.dbg

gcc ${OPTCFLAGS} waitforkey.c -lhiredis -o waitforkey.exe
gcc ${DBGCFLAGS} waitforkey.c -lhiredis -o waitforkey.dbg

strip *.exe
