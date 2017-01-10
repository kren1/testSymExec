#!/bin/bash
O_FILE=$(mktemp)
clang-3.4 -xc -I$CSMITH_RUNTIME -o $O_FILE $1 > /dev/null &&\
$O_FILE
rm $O_FILE
