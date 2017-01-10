#!/bin/bash
BC_FILE=$(mktemp)
clang-3.4 -xc -I$CSMITH_RUNTIME -o $BC_FILE -c -emit-llvm $1 > /dev/null &&\
timeout 100 klee $BC_FILE
EXIT_STATUS=$?
rm $BC_FILE
if [ $EXIT_STATUS == "124" ];
then
    echo "timeout"
    exit 0;
fi

if [ $EXIT_STATUS == "0" ];
then
    exit 0;
fi

exit 1
