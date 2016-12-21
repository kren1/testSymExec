#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/settings.sh
INSTRUMENT_TOOL=/usr/lib/llvm-3.9/bin/s2stool
$INSTRUMENT_TOOL -extra-arg="-I$CSMITH_RUNTIME" -h $1 --  > $2 
