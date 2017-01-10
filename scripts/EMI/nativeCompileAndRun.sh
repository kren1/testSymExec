#!/bin/bash
O_FILE=$(mktemp)
clang-3.4 -xc -I$CSMITH_RUNTIME -o $O_FILE $1 > /dev/null &&\
timeout 1 $O_FILE
EXIT_STATUS=$?
rm $O_FILE
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
