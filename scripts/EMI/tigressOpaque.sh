#!/bin/bash
DIR_NAME=$(dirname "$(realpath $0)")
source $DIR_NAME/../settings.sh
INSTRUMENT_TOOL=$GH_PROJECT_ROOT/src2src/build/tool
$TIGRESS --Transform=InitOpaque --Functions=* --Transform=AddOpaque --Functions=* --out=$2 $1

